OUT = sparry
#add -DLOG_MODE_ALL to enable logging
$(OUT):
	gcc \
		-DLOG_MODE_INFO \
		-DLOG_MODE_ERROR \
		-DLOG_MODE_TRACE \
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
