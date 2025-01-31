import ast

source_code = """
def greet(name):
    if name:
        print(f"Hello, {name}!")
    else:
        print("Hello, stranger!")

greet("cs6890")
"""

if __name__ == "__main__":
    tree = ast.parse(source_code)
    print(ast.dump(tree, indent=4))
