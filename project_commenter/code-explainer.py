import os

def check_commands():
    return

def decompile(prog_file):
    lines = []
    cmd = "bpftool dump xlated pinned " + prog_file + "> dumped.c"
    return lines
    
