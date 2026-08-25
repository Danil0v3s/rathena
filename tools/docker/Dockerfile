FROM alpine:3.23
WORKDIR /rathena
# zip/unzip/curl/pkgconf/ninja are required by vcpkg, which builds the dependencies declared in vcpkg.json
RUN apk add --no-cache wget curl zip unzip tar git cmake make ninja pkgconf gcc g++ gdb ca-certificates linux-headers bash valgrind netcat-openbsd
# vcpkg has no prebuilt tool for musl; build it from source
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
RUN wget https://raw.githubusercontent.com/eficode/wait-for/v2.2.4/wait-for -O /bin/wait-for && chmod +x /bin/wait-for
ENTRYPOINT [  ]
