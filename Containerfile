FROM ubuntu:18.04 as base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build clang build-essential zlib1g-dev
# RUN apt install nlohmann-json3-dev
RUN apt install -y wget
RUN apt install -y python python-dev git


# -----------------------------------------------------------------------------

FROM base_image as build_boost

WORKDIR /b
RUN wget https://archives.boost.io/release/1.68.0/source/boost_1_68_0.tar.bz2
RUN tar -xjf boost_1_68_0.tar.bz2
WORKDIR /b/boost_1_68_0
RUN ./bootstrap.sh --prefix=/b/boost/
RUN ./b2 install -j $(nproc)

# -----------------------------------------------------------------------------

FROM base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build clang build-essential
# RUN apt install nlohmann-json3-dev
RUN apt install -y wget
RUN apt install -y python python-dev git

WORKDIR /cmake
RUN wget https://cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.sh
RUN chmod +x ./cmake-3.8.2-Linux-x86_64.sh && echo "y" | ./cmake-3.8.2-Linux-x86_64.sh

WORKDIR /json
RUN git clone https://github.com/nlohmann/json.git
RUN cd json && git checkout 3.1.2

# WORKDIR /b
# RUN wget https://www.openssl.org/source/openssl-1.0.2g.tar.gz
# RUN tar -xzf openssl-1.0.2g.tar.gz
# WORKDIR /b/openssl-1.0.2g
# RUN ./config
# RUN make -j install

COPY --from=build_boost /b/boost /b/boost

ENV PATH "$PATH:/cmake/cmake-3.8.2-Linux-x86_64/bin"

# ENV CC=clang
# ENV CXX=clang
ENV CMAKE_COLOR_DIAGNOSTICS=ON

ENTRYPOINT /x/compile.sh
