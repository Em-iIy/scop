# Created by: Emily (Em_iIy) Winnink
# Created on: 06/09/2024

# ----------------------------------------Name
NAME = scop

# ----------------------------------------Files
FILES_SRCS =	main.cpp \
				glad.cpp \
				delta_time_update.cpp \
				print_controls.cpp \
				read_file.cpp \
				split.cpp \
				Window.cpp \
				Key.cpp \
				Shader.cpp \
				VAO.cpp \
				VBO.cpp \
				EBO.cpp \
				bmp.cpp \
				textures.cpp \
				Object.cpp \
				Camera.cpp \
				input.cpp \
				Manager.cpp \

FILES_OBJS = $(FILES_SRCS:.cpp=.o)

# ----------------------------------------Directories
DIR_SRCS = ./src/
DIR_OBJS = ./obj/
DIR_INC = ./inc/
DIR_EMLM = $(DIR_SRCS)lib/emlm/

vpath %.cpp	$(DIR_SRCS) \
			$(DIR_SRCS)lib/ \
			$(DIR_SRCS)lib/bmp/ \
			$(DIR_SRCS)utils/ \
			$(DIR_SRCS)utils/gl/ \

# ----------------------------------------Sources
SRCS = $(FILES_SRCS:%=$(DIR_SRCS)%)

# ----------------------------------------Objects
OBJS = $(FILES_OBJS:%=$(DIR_OBJS)%)

# ----------------------------------------Libs
EMLM = $(DIR_EMLM)libmlm.a

# ----------------------------------------Flags
CC = c++
CFLAGS = -std=c++11
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -O3
CFLAGS += -g -fsanitize=address

UNAME = $(shell uname)

# Linux flags
ifeq ($(UNAME), Linux)
	LFLAGS = -lglfw -ldl
endif
# Apple flags
ifeq ($(UNAME), Darwin)
	LFLAGS = -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
endif

INC =	-I$(DIR_INC) \
		-I$(DIR_SRCS) \
		-I$(DIR_SRCS)utils/ \
		-I$(DIR_SRCS)lib/ \

all:
	$(MAKE) $(NAME) -j4
.PHONY: all

$(NAME): $(DIR_OBJS) $(OBJS) $(EMLM)
	$(CC) $(OBJS) -o $(NAME) $(EMLM) $(CFLAGS) $(LFLAGS) $(INC)

$(DIR_OBJS)%.o: %.cpp
	$(CC) -c $< -o $@  $(CFLAGS) $(INC)

$(DIR_OBJS):
	mkdir -p $@

$(EMLM):
	$(MAKE) -j4 -C $(DIR_EMLM)

clean:
	$(MAKE) -C $(DIR_EMLM) clean
	rm -f $(OBJS)
.PHONY: clean

fclean: clean
	$(MAKE) -C $(DIR_EMLM) fclean
	rm -f $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re
