import calculator as calc

# crate instance of Harry
harry = calc.calculator(3,2)

# show what he can do
harry.a

harry.b

harry.add()

harry.substract()

harry.multiply()

harry.writeLinear()

harry.plotLinear()

calc.plotTwoColumnData("mylinFun_"+str(harry.a)+"_"+str(harry.b)+".dat")

print("Alles gemacht, was calculator harry kann :o) ")