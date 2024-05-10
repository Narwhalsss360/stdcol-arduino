'''Python script to arduinofy folder to use as arduino library'''

from os import listdir, remove
from os.path import isfile, isdir, join
from shutil import rmtree, move, copytree
from sys import argv

ARDUINO_EXAMPLES_DIR_NAME = 'Arduino'
EXAMPLES_DIR = 'examples'
INCLUDE_DIR = 'include'
SRC_DIR = 'src'

DELETE_FILES = [
    'stdcol.sln',
    'stdcol.vcxitems',
    'stdcol.vcxitems.filters',
    'makefile'
]

DELETE_TREES = [
    'include',
    'test',
    '.vscode'
]

def move_include():
    '''Move include -> src function'''

    for file_to_move in listdir(INCLUDE_DIR):
        if not isfile(join(INCLUDE_DIR, file_to_move)):
            continue

        try:
            move(join(INCLUDE_DIR, file_to_move), join(SRC_DIR, file_to_move))
        except OSError:
            print(f'There was an error moving {file_to_move}')

def move_examples():
    '''Move arduino examples to examples and delete non-arduino examples'''
    for item in listdir(EXAMPLES_DIR):
        item_dir = f'{EXAMPLES_DIR}/{item}'
        if item != ARDUINO_EXAMPLES_DIR_NAME:
            try:
                if isfile(item_dir):
                    remove(item_dir)
                else:
                    rmtree(item_dir)
            except OSError:
                print(f'There was an error deleting {item_dir=}')

    arduino_dir = f'{EXAMPLES_DIR}/{ARDUINO_EXAMPLES_DIR_NAME}'

    for item in listdir(arduino_dir):
        item_dir = f'{arduino_dir}/{item}'
        try:
            move(item_dir, f'{EXAMPLES_DIR}/{item}', copy_function=copytree)
        except OSError:
            print(f'There was an error moving {item_dir=}')

def delete_folders():
    '''Delete DELETE_TREES function'''

    for tree in DELETE_TREES:
        if not isdir(tree):
            print(f'Error! {tree=} is not a folder')
            continue

        try:
            rmtree(tree)
        except OSError:
            print(f'There was an error deleting {tree=}')

def delete_files():
    '''Delete DELETE_FILES function'''

    for file_to_delete in DELETE_FILES:
        if not isfile(file_to_delete):
            print(f'Error! {file_to_delete=} is not a file')
            continue

        try:
            remove(file_to_delete)
        except OSError:
            print(f'There was an error deleting file {file_to_delete}')

def main():
    '''Main module function'''

    move_include()
    move_examples()
    delete_folders()
    delete_files()

if __name__ == '__main__':
    main()
    remove(argv[0])
