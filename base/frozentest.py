print('Hello Playstation 2 Emotion Engine')
print('This is a microPython test')
print('Testing a string that has unicode αβγ chars')
print('Testing a sequence of bytes, numbers, and the range function')
print(b'bytes 1234\x01')
print(123456789)
for i in range(4):
    print(i)

print("Now Trying an import test")
try:
    import rpc
    print("Imported Successfully")
except ImportError:
    print("Import Failure")
except Exception as e:
    print(e)

print("Now Creating a Class")

class test(object):
    def __init__(self):
        self.propitem = None

    @property
    def prop(self):
        return self.propitem

    @prop.setter
    def prop(self, value):
        self.propitem = value

print("Class made")

print('Now entering an infinite loop so you can see the output')

while True:
    pass
