CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -fdiagnostics-color=always -lcurl
LDFLAGS=-g
LDLIBS=-lcurl

SRCS=connect4.cpp engine.cpp display.cpp input.cpp libs/joystick.c libs/led_matrix.c
OBJS=$(subst .cpp,.o,$(SRCS))

all: connect4

connect4: $(OBJS)
	$(CXX) $(LDFLAGS) -o connect4 $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend