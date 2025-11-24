#!/usr/bin/env python3
import sys
from recipes import NutritionAnalyzer, RecipeRecommender, RatingPredictor, GenerateMenu


def get_nutrients_recipes(ingredients_input):
    # ingredients = [item.strip().rstrip(',') for item in sys.argv[1:]]
    ingredients = [item.strip() for item in " ".join(ingredients_input).split(",") if item.strip()]

    predictor = RatingPredictor("data/clf.joblib", "data/feature_names.sav")
    nutrition = NutritionAnalyzer("data/ingredient_nutrient_dv.csv")
    recommender = RecipeRecommender("data/recipes_with_links.csv")

    forecast = predictor.predict(ingredients)
    print("I. OUR FORECAST")
    print(forecast)

    print("\nII. NUTRITION FACTS")
    facts = nutrition.get_nutrition(ingredients)
    for line in facts:
        print(line)

    print("\nIII. TOP-3 SIMILAR RECIPES:")
    similar = recommender.find_similar(ingredients)
    if isinstance(similar, str):  # вернулся текст вместо списка
        print(similar)
    else:
        for recipe in similar:
            print(f"- {recipe['title']}, rating: {recipe['rating']}, URL: {recipe['link']}")


def get_daily_menu():
    menu_generator = GenerateMenu("data/recipes_bonus.csv", "data/ingredient_nutrient_dv.csv")
    daily_menu = menu_generator.generate_daily_menu()
    menu_generator.print_daily_menu(daily_menu)


def main():
    if len(sys.argv) < 2:
        print("Введите ингредиенты через запятую: ./nutritionist.py ингредиент1, ингредиент2, ...")
        print("Для вызова генерации меню на день вызовите ./nutritionist.py menu")
        return

    user_input = sys.argv[1:]
    if user_input == ['menu']:
        get_daily_menu()
    else:
        get_nutrients_recipes(user_input)


if __name__ == "__main__":
    main()
