import sys

def AskHeight():
    try:
        heightFeet = input("Enter your height in Feet (e.g. 5'8\" Input \"5\"): ")
        heightFeet = int(heightFeet)
    except ValueError:
        print(f"Error: '{heightFeet}'  is not a valid Integer. Please enter a Integer value.\n")
        AskHeight()
    if (heightFeet <= 0):
        print(f"Error: '{heightFeet}'  is outside of expected values. Please enter new values.\n")
        AskHeight()
    try:
        heightInches = input("Enter your height in Inches (e.g. 5'8\" Input \"8\"): ")
        heightInches = int(heightInches)
    except ValueError:
        print(f"Error: '{heightInches}'  is not a valid Integer. Please enter a Integer value.\n")
        AskHeight()
    if (heightInches >= 12 or heightInches < 0):
        print(f"Error: '{heightInches}'  is outside of expected values. Please enter new values.\n")
        AskHeight()
    height = float(heightInches + heightFeet * 12)
    return height

def AskWeight():
    try:
        weight = input("Enter your weight in pounds: ")
        weight = float(weight)
    except ValueError:
        print(f"Error: '{weight}'  is not a valid number. Please enter a numeric value.\n")
        AskWeight()
    if (weight > 0):
        return weight
    else:
        print(f"Error: '{weight}'  is not a positive number. Please enter a positive value.\n")
        AskWeight()

def AskContinue():
    ans = input("Would you like to calculate again? (Y/N): ")

    if(ans == "Y"):
        return True
    elif(ans == "N"):
        return False
    else:
        print("\nInvalid Response, Please type either \"Y\" or \"N\".")
        return AskContinue()

def CalculateBMI(weight: float, height: float):
    BMI = (weight/(height*height)) * 703
    BMI = round(BMI, 1)
    if (BMI < 18.5): #18.5 expected
        category = "Underweight"
    elif(BMI < 25):
        category = "Normal Weight"
    elif(BMI < 30):
        category = "Overweight"
    else:
        category = "Obese"
    print(f"\nYour BMI is {f'{BMI}'}, and you are categorized as {category}\n")

def main():
    print(f"Welcome to the BMI Calculator. \nPlease answer these questions.\n")
    height = AskHeight()
    weight = AskWeight()
    CalculateBMI(weight, height)
    if(AskContinue()):
        main()
    else:
        print("\nThank you, Goodbye!")
        sys.exit(0)

main()