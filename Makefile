CC = gcc
PKG_CONFIG = pkg-config

#LINKER_FLAGS specifies the libraries we're linking against
LIBS = -lm \
       `$(PKG_CONFIG) --libs allegro-5` \
       `$(PKG_CONFIG) --libs allegro_primitives-5` \
       `$(PKG_CONFIG) --libs allegro_font-5` \
       `$(PKG_CONFIG) --libs allegro_ttf-5` \
       `$(PKG_CONFIG) --libs allegro_image-5` \
       `$(PKG_CONFIG) --libs allegro_audio-5` \
       `$(PKG_CONFIG) --libs allegro_acodec-5` \

OBJS = main.c
OBJ_NAME = draakblik

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(OBJ_NAME)
