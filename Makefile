OUT = sparry

$(OUT):
	gcc \
		-DLOG_MODE_ALL \
		src/main.c \
		src/raster.c \
		src/render.c \
		src/camera.c \
		src/transform.c \
		src/input.c \
		\
		-Iinclude\
		-lSDL2 -lm \
		-Wall -Wextra \
		-o $(OUT) 

run: $(OUT)
	./$(OUT)
