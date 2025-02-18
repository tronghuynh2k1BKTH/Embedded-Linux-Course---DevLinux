# PRACTICE SESSION 1

## BT1: Create a static library (.a)
## BT2: Create a shared library (.so)

**Note:** Use Makefile to automate all build steps.

---

## Some Information

- **fPIC significance in creating objects:**
  - Normally, machine code uses **absolute addresses**. But with PIC, the compiler uses **relative addresses**.
  - Instead of saying **"access address 0x4000"**, it will say **"access address 100 bytes from the current position"**.
  - This helps the code not be dependent on a fixed position in memory.

- **`rcs` are options for `ar`:**
  - `r` means to insert the files into the archive, replacing any existing files of the same name.
  - `c` means to create the archive if it does not already exist.
  - `s` means to create an index for the archive, which speeds up the linking process.

- **Distinguish between these two usages:**
  ```sh
  gcc $(OBJ_DIR)/main.o $(LIB_SHARED_DIR)/libhelloshared.so -o $(BIN_DIR)/shared_libs
  gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/static_libs
  ```

---