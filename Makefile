CLANG_FORMAT = clang-format
STYLE = Google
SRC_DIR = .
FILE_EXTENSIONS = h

# Find all C++ source and header files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( $(foreach ext, $(FILE_EXTENSIONS), -name '*.$(ext)') \))

# Default target
all:
	@echo "Use 'make format' to format all files."

# Format all files in place
format:
	@echo "Formatting files with clang-format ($(STYLE) style)..."
	@$(foreach file, $(SRC_FILES), $(CLANG_FORMAT) -i --style=$(STYLE) $(file);)
	@echo "Formatting complete."

# Dry run (preview changes without applying them)
format-dry-run:
	@echo "formating: $(SRC_FILES)"
	@echo "Previewing format changes with clang-format ($(STYLE) style)..."
	@$(foreach file, $(SRC_FILES), $(CLANG_FORMAT) --dry-run --style=$(STYLE) $(file);)
	@echo "Dry run complete."

# Clean (optional target for cleanup, e.g., if using build tools)
clean:
	@echo "Nothing to clean for this project."