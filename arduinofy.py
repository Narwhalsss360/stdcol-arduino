'''Python script to arduinofy folder to use as arduino library'''

from os import listdir, remove
from os.path import isfile, isdir, join
from shutil import rmtree, move
from sys import argv

INCLUDE_DIR = 'include'
SRC_DIR = 'src'

DELETE_FILES = [
    'stdcol.sln'
]

DELETE_TREES = [
    'include',
    'test'
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
    delete_folders()
    delete_files()

if __name__ == '__main__':
    main()
    remove(argv[0])
