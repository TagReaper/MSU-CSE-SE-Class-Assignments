heightFeet = None
heightInches = None
weight = None
BMI = None

def Ask():
    try:
        heightFeet = input("Enter your height in Feet (e.g. 5'8\" Input \"5\"): ")
        heightFeet = int(heightFeet)
    except ValueError:
        print(f"Error: '{heightFeet}'  is not a valid Integer. Please enter a Integer value.\n")
        Ask()
    try:
        heightInches = input("Enter your height in Feet (e.g. 5'8\" Input \"8\"): ")
        heightInches = int(heightInches)
    except ValueError:
        print(f"Error: '{heightInches}'  is not a valid Integer. Please enter a Integer value.\n")
        Ask()
    heightInches = float(heightInches + heightFeet * 12)
    try:
        weight = input("Enter your weight in pounds: ")
        weight = float(weight)
    except ValueError:
        print(f"Error: '{weight}'  is not a valid number. Please enter a numeric value.\n")
        Ask()

def CalculateBMI():
    BMI = (weight/(heightInches*heightInches)) * 703
    if (BMI < 18.5):
        category = "Underweight"
    elif(BMI < 25):
        category = "Normal Weight"
    elif(BMI < 30):
        category = "Overweight"
    else:
        category = "Obese"
    print(f"\nYour BMI is {f'{BMI:.1f}'}, and you are categorized as {category}")

def main():
    print(f"Welcome to the BMI Calculator. \n Please answer these 2 questions.\n")
    Ask()
    CalculateBMI()

main()