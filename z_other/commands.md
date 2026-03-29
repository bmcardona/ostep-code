# Best Practice (USE THIS)
gcc -Wall -Wextra -std=c17 filename.c -o filename && ./filename

# Flag Reference
-Wall          # Enable all standard warnings
-Wextra        # Enable extra warnings beyond -Wall
-std=c17       # Use C17 standard (c99, c11, c17, c2x available)
-g             # Include debugging symbols for gdb
-o filename    # Name the output executable
-c             # Compile only, don't link (creates .o file)
&&             # Run next command only if previous succeeds

# Common Commands
gcc filename.c -o filename        # Basic compile
./filename                        # Run executable
gcc -Wall -Wextra filename.c -o filename && ./filename    # Compile with warnings and run
gcc -Wall file1.c file2.c -o filename                     # Compile multiple files
gcc -c filename.c                 # Compile to filename.o object file
gcc filename.o -o filename        # Link object file to executable

# Compile and run (simple way)
gcc program.c -o program && ./program

