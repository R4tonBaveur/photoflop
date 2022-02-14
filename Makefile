CC = gcc
CFLAGS= -Wall -Wextra -std=c99 -O3 -fsanitize=address -g

# Libraries and path for linker
LDLIBS= `pkg-config --libs sdl2 gtk+-3.0` -lSDL2_image -lSDL2

# Linkers (usually empty)
LDFLAGS=-fsanitize=address

#FILES
SRCGUI=gui.c
SRCFILTERS=filters.c
SRCSELECT=select.c get_coord.c
SRCDRAW=draw.c
SRCIMAGE=image.c
OBJGUI = ${SRCGUI:%.c=%.o}
OBJFILTERS = ${SRCFILTERS:%.c=%.o}
OBJSELECT = ${SRCSELECT:%.c=%.o}
OBJDRAW = ${SRCDRAW:%.c=%.o}
OBJIMAGE = ${SRCIMAGE:%.c=%.o}

#RULES
gui:${OBJGUI} ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
filters:${OBJFILTERS}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
select:${OBJSELECT}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
draw:${OBJDRAW}  ${OBJIMAGE}
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
clean:
	${RM} ${OBJGUI} ${OBJIMAGE} ${OBJFILTERS} ${OBJSELECT} ${OBJDRAW} gui filters select draw
