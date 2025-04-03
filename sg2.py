import numpy as np


path="auto-mpg.csv"

data=np.genfromtxt(path,delimiter=',',dtype=str)

valid_data=data[~np.any(data=='?',axis=1)]

mpg=valid_data[1:,0].astype(float)
horsepower=valid_data[1:,3].astype(float)

print(mpg)
print(horsepower)

sorted_indices = np.argsort(horsepower)
horsepower = horsepower[sorted_indices]
mpg = mpg[sorted_indices]

coef = np.polyfit(horsepower, mpg, deg=2)
print(coef)
poly_eq = np.poly1d(coef)
regression_line = poly_eq(horsepower)

print(regression_line)

np.savetxt("cleaned_data.csv", np.column_stack((mpg, horsepower,regression_line)), delimiter=",", header="mpg,horsepower,regression_line", comments="", fmt="%f")
