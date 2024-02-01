NAME		=	scop
SRC			=	VeDevice.cpp VePipeline.cpp VeWindow.cpp main.cpp

# Commands
CPP			=	c++
RM			=	rm -rf
GLSLC		=	./shader/glslc

# Flags
CPPFLAGS	=	-std=c++17 -O2 -MMD
LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Path
INC			=	-I ./inc/
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/

# Objects
OBJ_DIRS	=	${OBJ_PATH}
OBJ			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.o}}
DEP			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.d}}

all:${NAME}

clean:
	${RM} ${OBJ_PATH}

fclean:clean
	${RM} ${NAME} ./shader/*.spv

re:fclean
	make all

run:${NAME}
	clear
	./${NAME}

${OBJ_PATH}%.o:${SRC_PATH}%.cpp
	${CPP} ${CPPFLAGS} ${INC} -c $< -o $@

${OBJ_DIRS}:
	mkdir ${OBJ_DIRS}

${NAME}:${OBJ_DIRS} ${OBJ}
	${CPP} ${OBJ} ${LDFLAGS} -o $@
	${GLSLC} ./shader/simpleShader.vert -o ./shader/simpleShader.vert.spv
	${GLSLC} ./shader/simpleShader.frag -o ./shader/simpleShader.frag.spv

.PHONY:all clean fclean re run

-include ${DEP}
