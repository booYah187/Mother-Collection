CC ?= cc
PKG_CONFIG ?= pkg-config
TARGET ?= launcher

SDL_CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer 2>/dev/null)
SDL_LIBS   := $(shell $(PKG_CONFIG) --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer 2>/dev/null)

CPPFLAGS += -Isrc $(SDL_CFLAGS)
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS += $(SDL_LIBS) -lm

SOURCES := src/main.c src/launcher.c src/audio.c src/launch.c src/hash.c
OBJECTS := $(SOURCES:.c=.o)

.PHONY: all clean print-config
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

print-config:
	@echo "CC=$(CC)"
	@echo "PKG_CONFIG=$(PKG_CONFIG)"
	@echo "SDL_CFLAGS=$(SDL_CFLAGS)"
	@echo "SDL_LIBS=$(SDL_LIBS)"
