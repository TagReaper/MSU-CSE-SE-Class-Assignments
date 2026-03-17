height = None
weight = None
BMI = None

def Ask():
    height = float(input("Enter your height in inches: "))
    weight = float(input("Enter your height in pounds: "))

def BMI():
    BMI = (weight/(height*height)) * 703

def main():
    print(f"Welcome to the BMI Calculator. \n Please answer these 2 questions.\n")
    Ask()
    while (height):
        pass

main()