class MathsOperations:
    def __init__ (self, x, y):
        self.a = x
        self.b = y
    def testAddition(self, c, augment=False, visualize=False):
        print('c', c)
        print('augment : ', augment)
        return (self.a + self.b)

    # def testMultiplication(self):
    #     return (self.a * self.b)

a = MathsOperations(5,6)
# b = a()
print(a.testAddition(3))