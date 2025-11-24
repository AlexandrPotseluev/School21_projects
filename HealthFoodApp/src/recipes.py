import pandas as pd
import joblib
import re
from math import ceil
import random


class NutritionAnalyzer:
    def __init__(self, nutrition_file):
        self.data = pd.read_csv(nutrition_file)

    def get_nutrition(self, ingredients):
        results = []
        missing_ingredients = []
        ingredients = [ingredient.strip().lower() for ingredient in ingredients]

        for ingredient in ingredients:
            ingredient_data = self.data[self.data['Ingredient'] == ingredient]

            if ingredient_data.empty:
                missing_ingredients.append(ingredient)
                # results.append(f"{ingredient.capitalize()} - Ingredient not found in database")
                # results.append("")  # Пустая строка
                continue

            results.append(ingredient.capitalize())
            row = ingredient_data.iloc[0]
            for column in self.data.columns:
                if column != 'Ingredient' and pd.notna(row[column]) and row[column] != 0:
                    results.append(f"{column} - {row[column]}% of Daily Value")

            results.append("")  # Пустая строка между ингредиентами

        if missing_ingredients:
            missing_list = ", ".join([ing.capitalize() for ing in missing_ingredients])
            results.append(f"Sorry, the following ingredients not found in database: {missing_list}")
            results.append("")

        return results


class RatingPredictor:
    def __init__(self, model_file, feature_file):
        self.model = joblib.load(model_file)
        self.feature_names = joblib.load(feature_file)
        self.pred_texts = {
            "bad": (
                "You might find it tasty, but in our opinion, it is a bad idea "
                "to have a dish with that list of ingredients."
            ),
            "so-so": (
                "So-so. This dish could work, but some flavors or nutrients "
                "might not harmonize perfectly. You can try it, but maybe tweak "
                "the ingredients for better taste and balance."
            ),
            "great": (
                "Great choice! This combination looks both healthy and delicious. "
                "It’s well-balanced, likely to taste good, and offers a nice nutritional profile."
            )
        }

    def predict(self, ingredients):
        # готовим пустой DataFrame со всеми признаками
        X_input = pd.DataFrame(
            [[1 if col in ingredients else 0 for col in self.feature_names]],
            columns=self.feature_names
        )
        pred = self.model.predict(X_input)[0]
        return self.pred_texts.get(pred, "Prediction unknown")


class RecipeRecommender:
    def __init__(self, recipes_file):
        self.data = pd.read_csv(recipes_file)

    def find_similar(self, ingredients, top_n=3, max_extra=3, min_fraction=1/3):
        ingredients = [ing.lower().strip() for ing in ingredients]
        df = self.data.loc[self.data['recipe_link'].notna()].copy()

        ingredient_cols = set(self.data.columns) - {'title', 'rating', 'recipe_link', 'round_rating', 'cat_rating'}

        results = []
        min_required = max(1, ceil(len(ingredients) * min_fraction))  # минимум совпадений

        for _, row in df.iterrows():
            recipe_ings = [col for col in ingredient_cols if row.get(col, 0) == 1]

            matched_ings = sum(1 for ing in ingredients if ing in recipe_ings)
            if matched_ings < min_required:
                continue  # не дотянул до доли совпадений

            total_ings = len(recipe_ings)
            extra = total_ings - matched_ings
            if extra > max_extra:
                continue

            # совпадения в названии
            title_lower = row['title'].lower()
            match_in_title = sum(
                bool(re.search(rf'\b{re.escape(ing)}\b', title_lower) or re.search(rf'{re.escape(ing)}-', title_lower))
                for ing in ingredients
            )

            results.append({
                'title': row['title'],
                'rating': row['rating'],
                'link': row['recipe_link'],
                'matched_ings': matched_ings,
                'extra': extra,
                'match_in_title': match_in_title
            })

        if not results:
            return "There are no similar recipes"

        results_sorted = sorted(
            results,
            key=lambda x: (x['match_in_title'], -x['extra'], x['matched_ings'], x['rating']),
            reverse=True
        )

        return results_sorted[:top_n]


class GenerateMenu:
    def __init__(self, recipes_file, daily_values_file, recipe_weight=300):
        self.data = pd.read_csv(recipes_file, index_col=0)
        self.daily_norms = pd.read_csv(daily_values_file, index_col=0)
        self.nutrient_cols = self.daily_norms.columns.tolist()
        self.recipe_weight = recipe_weight

        # ключевые нутриенты для проверки покрытия
        self.key_nutrients = [
            "Protein", "Total Fat", "Carbohydrate", "Fiber", "Sodium"
        ]

        exclude_cols = ['title', 'rating', 'recipe_link', 'round_rating', 'cat_rating',
                        'breakfast', 'lunch', 'dinner']
        self.ingredient_cols = [col for col in self.data.columns if col not in exclude_cols]

    def recipe_ingredients(self, recipe_title):
        """Возвращает список ингредиентов для рецепта"""
        row = self.data.loc[recipe_title]
        return [col for col in self.ingredient_cols if row.get(col, 0) == 1]

    def recipe_nutrition_percent(self, recipe_title):
        """Считает нутриенты для одного рецепта с учётом веса блюда"""
        ingredients = self.recipe_ingredients(recipe_title)
        if not ingredients:
            return pd.Series(0, index=self.nutrient_cols)

        grams_per_ing = self.recipe_weight / len(ingredients)
        percents = pd.Series(0.0, index=self.nutrient_cols)
        for ing in ingredients:
            if ing in self.daily_norms.index:
                percents += self.daily_norms.loc[ing] * (grams_per_ing / 100.0)
        return percents

    def nutrients_in_range(self, total_nutrients):
        """Проверка, что суммарные нутриенты не превышают 100%"""
        for nut in self.key_nutrients:
            if total_nutrients[nut] > 100:
                return False
        return True

    def generate_daily_menu(self, top_n=5, max_attempts=500):
        """
        Оптимизированный подбор меню:
        - Сначала берём случайные top_n кандидатов для каждого приёма пищи
        - Проверяем комбинации
        - При необходимости добавляем новые кандидаты
        """
        # кэшируем нутриенты всех рецептов
        recipe_cache = {title: self.recipe_nutrition_percent(title) for title in self.data.index}

        # делим рецепты по типу приёма пищи
        breakfast_recipes = [idx for idx in self.data.index if self.data.loc[idx, "breakfast"] == 1]
        lunch_recipes = [idx for idx in self.data.index if self.data.loc[idx, "lunch"] == 1]
        dinner_recipes = [idx for idx in self.data.index if self.data.loc[idx, "dinner"] == 1]

        best_menu = None
        best_rating = -1

        attempts = 0
        while attempts < max_attempts:
            attempts += 1

            # случайные кандидаты
            breakfast_candidates = random.sample(breakfast_recipes, min(top_n, len(breakfast_recipes)))
            lunch_candidates = random.sample(lunch_recipes, min(top_n, len(lunch_recipes)))
            dinner_candidates = random.sample(dinner_recipes, min(top_n, len(dinner_recipes)))

            # перебираем все комбинации
            for b in breakfast_candidates:
                for l in lunch_candidates:
                    for d in dinner_candidates:
                        total_nutrients = recipe_cache[b] + recipe_cache[l] + recipe_cache[d]

                        if not self.nutrients_in_range(total_nutrients):
                            continue

                        total_rating = self.data.loc[b, "rating"] + self.data.loc[l, "rating"] + self.data.loc[d, "rating"]

                        if total_rating > best_rating:
                            best_rating = total_rating
                            best_menu = {"breakfast": b, "lunch": l, "dinner": d}
            if best_menu:
                return best_menu
        return best_menu

    def print_daily_menu(self, menu):
        """Красивый вывод меню с нутриентами каждого блюда"""
        total_nutrients = pd.Series(0.0, index=self.nutrient_cols)
        total_rating = 0

        print("\nDAILY MENU")
        print("=" * 50)

        for meal in ["breakfast", "lunch", "dinner"]:
            recipe_title = menu.get(meal)
            print(f"\n{meal.upper()}")
            print("-" * 20)

            if not recipe_title:
                print("No suitable recipe found")
                continue

            row = self.data.loc[recipe_title]
            nutrients = self.recipe_nutrition_percent(recipe_title)
            total_nutrients += nutrients
            total_rating += row['rating']

            print(f"{recipe_title}  (rating: {row['rating']:.2f})")
            print("Ingredients:")
            for ing in self.recipe_ingredients(recipe_title):
                print(f"- {ing}")

            print("Nutrients:")
            for nut in self.key_nutrients:
                if nutrients[nut] > 0:
                    print(f"- {nut}: {nutrients[nut]:.1f}%")

            if pd.notna(row.get("recipe_link", None)):
                print(f"URL: {row['recipe_link']}")

        # # суммарные нутриенты
        # print("\n" + "=" * 50)
        # print("TOTAL DAILY NUTRIENTS:")
        # print("-" * 30)
        # for nut in self.key_nutrients:
        #     print(f"- {nut}: {total_nutrients[nut]:.1f}%")
        # print(f"Total rating: {total_rating:.2f}")
