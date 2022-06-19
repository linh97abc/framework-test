# print("hello")

# import unittest

# class abc(unittest.TestCase):
#     def test_1(self):
#         assert 1 == 0
#         pass

# unittest.main(verbosity=2)

TC_COLOR_RED = "\x1b[1;31m"
TC_COLOR_GREEN = "\x1b[1;32m"
TC_COLOR_YELLOW = "\x1b[1;33m"
TC_COLOR_BLUE = "\x1b[1;34m"
TC_COLOR_END = "\x1b[1;39m"

with open("run.log", 'r') as f:
    s = f.read()
    s = s.replace(TC_COLOR_RED, '')
    s = s.replace(TC_COLOR_GREEN, '')
    s = s.replace(TC_COLOR_YELLOW, '')
    s = s.replace(TC_COLOR_BLUE, '')
    s = s.replace(TC_COLOR_END, '')

    print(s)
