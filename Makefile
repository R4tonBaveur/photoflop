CC = gcc
# Flags for the compilation
CFLAGS= `pkg-config --cflags sdl2 gtk+-3.0` -lSDL2_image -Wall -Wextra -std=c99 -O3 -g -rdynamic
# Libraries and path for linker
LDLIBS= `pkg-config --libs sdl2 gtk+-3.0` -lSDL2_image


# Linkers (usually empty)
LDFLAGS=

#FILES
SRCGUI = gui.c
SRCFILTERS = filters/grayscale.c
SRCSELECT = select.c
SRCDRAW = draw.c
SRCIMAGE = image.c
SRCUTILS = C-Utils/stack/stack.c C-Utils/queue/queue.c
OBJGUI = ${SRCGUI:%.c=%.o}
OBJFILTERS = ${SRCFILTERS:%.c=%.o}
OBJSELECT = ${SRCSELECT:%.c=%.o}
OBJDRAW = ${SRCDRAW:%.c=%.o}
OBJIMAGE = ${SRCIMAGE:%.c=%.o}
OBJUTILS = ${SRCUTILS:%.c=%.o}

#RULES
gui:${OBJGUI} ${OBJIMAGE} ${OBJUTILS} ${OBJFILTERS}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
filters:${OBJFILTERS}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
select:${OBJSELECT}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
draw:${OBJDRAW}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
clean:
	${RM} *.o gui filters select draw
