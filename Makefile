NAME = fov

UNAME_S = $(shell uname -s)

SRCDIR = srcs/

OBJDIR = .obj/

DEPDIR = .dep/

INCLUDEDIR = include/

FLAGS = -Wall -Werror -Wextra --std=c++14

SOURCEFILES =	main.cpp \
				Config.cpp

SOURCE = $(addprefix $(SRCDIR), $(SOURCEFILES))

OBJ = $(addprefix $(OBJDIR), $(SOURCEFILES:.cpp=.o))

DEP = $(addprefix $(DEPDIR), $(SOURCEFILES:.cpp=.d))

INCLUDES = -I $(INCLUDEDIR)

ifeq ($(UNAME_S),Darwin)
LIBS = -L ./libs/glfw-3.3.5/src/ -lglfw3 -lpthread -ldl -lm -L ./libs/
else
LIBS = -L ./libs/glfw-3.3.5/src/ -lglfw3 -lpthread -ldl -lm -lGL -lX11 -L ./libs/
endif

all: $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p $(dir $@)
	clang++ $(FLAGS) -c $< -o $@ $(INCLUDES) -MMD -MF $(DEPDIR)$*.d

$(OBJDIR)glad.o: libs/glad/src/glad.c
	mkdir -p $(dir $@)
	gcc -c $< -o .obj/glad.o $(INCLUDES) -MMD -MF $(DEPDIR)glad.d

$(OBJDIR):
	mkdir $(OBJDIR)

$(DEPDIR):
	mkdir $(DEPDIR)

$(OBJ): | $(OBJDIR) $(DEPDIR)

$(NAME): $(OBJ)
ifeq ($(UNAME_S),Darwin)
	clang++ $(OBJ) -o $(NAME) -framework Cocoa -framework OpenGL -framework QuartzCore -framework IOKit $(LIBS)
else
	clang++ $(OBJ) -o $(NAME)
endif

clean:
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all

-include $(DEP)
