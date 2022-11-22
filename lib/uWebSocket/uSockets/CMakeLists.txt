
#add_subdirectory(lsquic)

include_directories(
        src
)

add_library(
        uSockets
        STATIC
        src/bsd.c
        src/context.c
        src/eventing/epoll_kqueue.c
        src/eventing/gcd.c
        src/eventing/libuv.c

        src/loop.c
        src/crypto/openssl.c

        src/quic.h
        src/quic.c
        src/socket.c
        src/udp.c

        src/internal/internal.h

)

