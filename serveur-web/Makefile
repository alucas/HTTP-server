PREFIX=.

INCDIR = $(PREFIX)
LIBDIR = $(PREFIX)
BINDIR = $(PREFIX)

CC= gcc
CFLAGS= -std=c99 -g -Wall -O3 -D_REENTRANT -DNDEBUG
CPPFLAGS= -I $(INCDIR)
LDFLAGS= -L $(LIBDIR)
LDLIBS= -lm -lpthread -lcurses
LIBFILENAME= 
INSTHEADERS= 
LIBOBJS= 
OUTFILE= lina
OBJS= $(LIBOBJS) serveur.o configuration.o encode.o decode.o makeLog.o authentification.o getContentType.o requete.o memoire.o buffer.o sendrecv.o client.o
SRCS= serveur.c configuration.c encode.c decode.c makeLog.c authentification.c getContentType.c requete.c memoire.c buffer.c sendrecv.c client.c

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(OUTFILE)

clean :
	rm -f  $(OUTFILE) $(OBJS)
	rm -f *~

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)


# DO NOT DELETE

serveur.o: client.h buffer.h memoire.h /usr/include/pthread.h
serveur.o: /usr/include/sys/types.h /usr/include/_ansi.h
serveur.o: /usr/include/newlib.h /usr/include/sys/config.h
serveur.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
serveur.o: /usr/include/machine/_types.h
serveur.o: /usr/include/machine/_default_types.h /usr/include/sys/_types.h
serveur.o: /usr/include/sys/lock.h /usr/include/machine/types.h
serveur.o: /usr/include/signal.h /usr/include/sys/signal.h
serveur.o: /usr/include/sched.h /usr/include/sys/sched.h /usr/include/time.h
serveur.o: /usr/include/sys/reent.h /usr/include/machine/time.h
serveur.o: /usr/include/arpa/inet.h /usr/include/netinet/in.h
serveur.o: /usr/include/cygwin/in.h /usr/include/cygwin/socket.h
serveur.o: /usr/include/stdint.h /usr/include/asm/socket.h
serveur.o: /usr/include/cygwin/if.h /usr/include/sys/socket.h
serveur.o: /usr/include/features.h /usr/include/sys/cdefs.h
serveur.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
serveur.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
serveur.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h global.h
serveur.o: configuration.h requete.h sendrecv.h /usr/include/stdlib.h
serveur.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
serveur.o: /usr/include/stdio.h /usr/include/sys/stdio.h
serveur.o: /usr/include/unistd.h /usr/include/sys/unistd.h
serveur.o: /usr/include/string.h /usr/include/sys/string.h
serveur.o: /usr/include/locale.h /usr/include/errno.h
serveur.o: /usr/include/sys/errno.h /usr/include/limits.h
serveur.o: /usr/include/sys/select.h /usr/include/netdb.h
serveur.o: /usr/include/inttypes.h
configuration.o: /usr/include/stdlib.h /usr/include/machine/ieeefp.h
configuration.o: /usr/include/_ansi.h /usr/include/newlib.h
configuration.o: /usr/include/sys/config.h /usr/include/sys/features.h
configuration.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
configuration.o: /usr/include/machine/_types.h
configuration.o: /usr/include/machine/_default_types.h
configuration.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
configuration.o: /usr/include/alloca.h /usr/include/stdio.h
configuration.o: /usr/include/sys/types.h /usr/include/machine/types.h
configuration.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
configuration.o: /usr/include/unistd.h /usr/include/sys/unistd.h
configuration.o: /usr/include/string.h /usr/include/sys/string.h
configuration.o: /usr/include/netdb.h /usr/include/inttypes.h
configuration.o: /usr/include/stdint.h /usr/include/sys/socket.h
configuration.o: /usr/include/features.h /usr/include/cygwin/socket.h
configuration.o: /usr/include/asm/socket.h /usr/include/cygwin/if.h
configuration.o: /usr/include/cygwin/sockios.h /usr/include/sys/uio.h
configuration.o: /usr/include/sys/time.h /usr/include/netinet/in.h
configuration.o: /usr/include/cygwin/in.h /usr/include/asm/byteorder.h
configuration.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h
configuration.o: /usr/include/errno.h /usr/include/sys/errno.h
configuration.o: /usr/include/sys/stat.h /usr/include/time.h
configuration.o: /usr/include/machine/time.h /usr/include/fcntl.h
configuration.o: /usr/include/sys/fcntl.h /usr/include/sys/_default_fcntl.h
configuration.o: global.h buffer.h memoire.h configuration.h
encode.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
encode.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
encode.o: /usr/include/sys/features.h /usr/include/sys/reent.h
encode.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
encode.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
encode.o: /usr/include/sys/types.h /usr/include/machine/types.h
encode.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
encode.o: /usr/include/string.h /usr/include/sys/string.h /usr/include/time.h
encode.o: /usr/include/machine/time.h encode.h buffer.h memoire.h requete.h
encode.o: client.h /usr/include/pthread.h /usr/include/signal.h
encode.o: /usr/include/sys/signal.h /usr/include/sched.h
encode.o: /usr/include/sys/sched.h /usr/include/arpa/inet.h
encode.o: /usr/include/netinet/in.h /usr/include/cygwin/in.h
encode.o: /usr/include/cygwin/socket.h /usr/include/stdint.h
encode.o: /usr/include/asm/socket.h /usr/include/cygwin/if.h
encode.o: /usr/include/sys/socket.h /usr/include/features.h
encode.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
encode.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
encode.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h global.h
decode.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
decode.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
decode.o: /usr/include/sys/features.h /usr/include/sys/reent.h
decode.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
decode.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
decode.o: /usr/include/sys/types.h /usr/include/machine/types.h
decode.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
decode.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
decode.o: /usr/include/alloca.h /usr/include/string.h
decode.o: /usr/include/sys/string.h /usr/include/math.h decode.h buffer.h
decode.o: memoire.h requete.h client.h /usr/include/pthread.h
decode.o: /usr/include/signal.h /usr/include/sys/signal.h
decode.o: /usr/include/sched.h /usr/include/sys/sched.h /usr/include/time.h
decode.o: /usr/include/machine/time.h /usr/include/arpa/inet.h
decode.o: /usr/include/netinet/in.h /usr/include/cygwin/in.h
decode.o: /usr/include/cygwin/socket.h /usr/include/stdint.h
decode.o: /usr/include/asm/socket.h /usr/include/cygwin/if.h
decode.o: /usr/include/sys/socket.h /usr/include/features.h
decode.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
decode.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
decode.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h
makeLog.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
makeLog.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
makeLog.o: /usr/include/sys/features.h /usr/include/sys/reent.h
makeLog.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
makeLog.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
makeLog.o: /usr/include/sys/types.h /usr/include/machine/types.h
makeLog.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
makeLog.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
makeLog.o: /usr/include/alloca.h /usr/include/string.h
makeLog.o: /usr/include/sys/string.h /usr/include/time.h
makeLog.o: /usr/include/machine/time.h makeLog.h requete.h client.h buffer.h
makeLog.o: memoire.h /usr/include/pthread.h /usr/include/signal.h
makeLog.o: /usr/include/sys/signal.h /usr/include/sched.h
makeLog.o: /usr/include/sys/sched.h /usr/include/arpa/inet.h
makeLog.o: /usr/include/netinet/in.h /usr/include/cygwin/in.h
makeLog.o: /usr/include/cygwin/socket.h /usr/include/stdint.h
makeLog.o: /usr/include/asm/socket.h /usr/include/cygwin/if.h
makeLog.o: /usr/include/sys/socket.h /usr/include/features.h
makeLog.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
makeLog.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
makeLog.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h global.h
authentification.o: /usr/include/stdlib.h /usr/include/machine/ieeefp.h
authentification.o: /usr/include/_ansi.h /usr/include/newlib.h
authentification.o: /usr/include/sys/config.h /usr/include/sys/features.h
authentification.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
authentification.o: /usr/include/machine/_types.h
authentification.o: /usr/include/machine/_default_types.h
authentification.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
authentification.o: /usr/include/alloca.h /usr/include/stdio.h
authentification.o: /usr/include/sys/types.h /usr/include/machine/types.h
authentification.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
authentification.o: /usr/include/string.h /usr/include/sys/string.h
authentification.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
authentification.o: /usr/include/sys/time.h /usr/include/cygwin/wait.h
authentification.o: /usr/include/unistd.h /usr/include/sys/unistd.h
authentification.o: authentification.h global.h buffer.h memoire.h
getContentType.o: getContentType.h requete.h client.h buffer.h memoire.h
getContentType.o: /usr/include/pthread.h /usr/include/sys/types.h
getContentType.o: /usr/include/_ansi.h /usr/include/newlib.h
getContentType.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
getContentType.o: /usr/include/sys/features.h /usr/include/machine/_types.h
getContentType.o: /usr/include/machine/_default_types.h
getContentType.o: /usr/include/sys/_types.h /usr/include/sys/lock.h
getContentType.o: /usr/include/machine/types.h /usr/include/signal.h
getContentType.o: /usr/include/sys/signal.h /usr/include/sched.h
getContentType.o: /usr/include/sys/sched.h /usr/include/time.h
getContentType.o: /usr/include/sys/reent.h /usr/include/machine/time.h
getContentType.o: /usr/include/arpa/inet.h /usr/include/netinet/in.h
getContentType.o: /usr/include/cygwin/in.h /usr/include/cygwin/socket.h
getContentType.o: /usr/include/stdint.h /usr/include/asm/socket.h
getContentType.o: /usr/include/cygwin/if.h /usr/include/sys/socket.h
getContentType.o: /usr/include/features.h /usr/include/sys/cdefs.h
getContentType.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
getContentType.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
getContentType.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h
getContentType.o: global.h /usr/include/sys/stat.h /usr/include/unistd.h
getContentType.o: /usr/include/sys/unistd.h /usr/include/stdio.h
getContentType.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
getContentType.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
getContentType.o: /usr/include/string.h /usr/include/sys/string.h
requete.o: global.h buffer.h memoire.h requete.h client.h
requete.o: /usr/include/pthread.h /usr/include/sys/types.h
requete.o: /usr/include/_ansi.h /usr/include/newlib.h
requete.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
requete.o: /usr/include/sys/features.h /usr/include/machine/_types.h
requete.o: /usr/include/machine/_default_types.h /usr/include/sys/_types.h
requete.o: /usr/include/sys/lock.h /usr/include/machine/types.h
requete.o: /usr/include/signal.h /usr/include/sys/signal.h
requete.o: /usr/include/sched.h /usr/include/sys/sched.h /usr/include/time.h
requete.o: /usr/include/sys/reent.h /usr/include/machine/time.h
requete.o: /usr/include/arpa/inet.h /usr/include/netinet/in.h
requete.o: /usr/include/cygwin/in.h /usr/include/cygwin/socket.h
requete.o: /usr/include/stdint.h /usr/include/asm/socket.h
requete.o: /usr/include/cygwin/if.h /usr/include/sys/socket.h
requete.o: /usr/include/features.h /usr/include/sys/cdefs.h
requete.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
requete.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
requete.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h sendrecv.h
requete.o: getContentType.h /usr/include/sys/stat.h /usr/include/unistd.h
requete.o: /usr/include/sys/unistd.h authentification.h makeLog.h decode.h
requete.o: encode.h /usr/include/stdlib.h /usr/include/machine/stdlib.h
requete.o: /usr/include/alloca.h /usr/include/stdio.h
requete.o: /usr/include/sys/stdio.h /usr/include/string.h
requete.o: /usr/include/sys/string.h /usr/include/assert.h
requete.o: /usr/include/limits.h
memoire.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
memoire.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
memoire.o: /usr/include/sys/features.h /usr/include/sys/reent.h
memoire.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
memoire.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
memoire.o: /usr/include/sys/types.h /usr/include/machine/types.h
memoire.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h
memoire.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
memoire.o: /usr/include/alloca.h memoire.h
buffer.o: /usr/include/stdlib.h /usr/include/machine/ieeefp.h
buffer.o: /usr/include/_ansi.h /usr/include/newlib.h
buffer.o: /usr/include/sys/config.h /usr/include/sys/features.h
buffer.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
buffer.o: /usr/include/machine/_types.h /usr/include/machine/_default_types.h
buffer.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
buffer.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/sys/types.h
buffer.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
buffer.o: /usr/include/sys/cdefs.h /usr/include/string.h
buffer.o: /usr/include/sys/string.h /usr/include/assert.h buffer.h memoire.h
sendrecv.o: global.h buffer.h memoire.h sendrecv.h /usr/include/stdlib.h
sendrecv.o: /usr/include/machine/ieeefp.h /usr/include/_ansi.h
sendrecv.o: /usr/include/newlib.h /usr/include/sys/config.h
sendrecv.o: /usr/include/sys/features.h /usr/include/sys/reent.h
sendrecv.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
sendrecv.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
sendrecv.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
sendrecv.o: /usr/include/stdio.h /usr/include/sys/types.h
sendrecv.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
sendrecv.o: /usr/include/sys/cdefs.h /usr/include/assert.h
sendrecv.o: /usr/include/dirent.h /usr/include/sys/dirent.h
sendrecv.o: /usr/include/limits.h /usr/include/features.h
sendrecv.o: /usr/include/string.h /usr/include/sys/string.h
sendrecv.o: /usr/include/sys/socket.h /usr/include/cygwin/socket.h
sendrecv.o: /usr/include/stdint.h /usr/include/asm/socket.h
sendrecv.o: /usr/include/cygwin/if.h /usr/include/cygwin/sockios.h
sendrecv.o: /usr/include/sys/uio.h /usr/include/sys/time.h
sendrecv.o: /usr/include/errno.h /usr/include/sys/errno.h
sendrecv.o: /usr/include/unistd.h /usr/include/sys/unistd.h
sendrecv.o: /usr/include/time.h /usr/include/machine/time.h
sendrecv.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
sendrecv.o: /usr/include/cygwin/wait.h
client.o: client.h buffer.h memoire.h /usr/include/pthread.h
client.o: /usr/include/sys/types.h /usr/include/_ansi.h /usr/include/newlib.h
client.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
client.o: /usr/include/sys/features.h /usr/include/machine/_types.h
client.o: /usr/include/machine/_default_types.h /usr/include/sys/_types.h
client.o: /usr/include/sys/lock.h /usr/include/machine/types.h
client.o: /usr/include/signal.h /usr/include/sys/signal.h
client.o: /usr/include/sched.h /usr/include/sys/sched.h /usr/include/time.h
client.o: /usr/include/sys/reent.h /usr/include/machine/time.h
client.o: /usr/include/arpa/inet.h /usr/include/netinet/in.h
client.o: /usr/include/cygwin/in.h /usr/include/cygwin/socket.h
client.o: /usr/include/stdint.h /usr/include/asm/socket.h
client.o: /usr/include/cygwin/if.h /usr/include/sys/socket.h
client.o: /usr/include/features.h /usr/include/sys/cdefs.h
client.o: /usr/include/sys/time.h /usr/include/cygwin/sockios.h
client.o: /usr/include/sys/uio.h /usr/include/asm/byteorder.h
client.o: /usr/include/bits/endian.h /usr/include/cygwin/in6.h global.h
client.o: /usr/include/stdio.h /usr/include/sys/stdio.h /usr/include/netdb.h
client.o: /usr/include/inttypes.h /usr/include/unistd.h
client.o: /usr/include/sys/unistd.h /usr/include/string.h
client.o: /usr/include/sys/string.h
