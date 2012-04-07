from PIL import Image
from PIL import ImageFilter
from PIL import PSDraw
import random


om = Image.new("RGB", (400,400))
im = Image.open("car.jpg")

width, height = im.size
print(width)
print(height)
left = random.randrange(width-400)
right = left + 400
top = random.randrange(height-400)
bottom = top + 400

om = im.crop((left, top, right, bottom)).save("croppedCar.jpg")


