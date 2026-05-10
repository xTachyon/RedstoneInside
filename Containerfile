FROM ubuntu:20.04 as base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build build-essential zlib1g-dev
# RUN apt install nlohmann-json3-dev
RUN apt install -y wget
RUN apt install -y python3 python3-dev git


# -----------------------------------------------------------------------------

FROM base_image as build_boost

WORKDIR /b
RUN wget https://archives.boost.io/release/1.69.0/source/boost_1_69_0.tar.bz2
RUN tar -xjf boost_1_69_0.tar.bz2
WORKDIR /b/boost_1_69_0
RUN ./bootstrap.sh --prefix=/b/boost/
RUN ./b2 install -j $(nproc)

# -----------------------------------------------------------------------------

FROM base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build build-essential cmake
# RUN apt install nlohmann-json3-dev
RUN apt install -y wget
RUN apt install -y python3 python3-dev git

WORKDIR /json
RUN git clone https://github.com/nlohmann/json.git
RUN cd json && git checkout 3.1.2

COPY --from=build_boost /b/boost /b/boost

# ENV PATH "$PATH:/cmake/cmake-3.8.2-Linux-x86_64/bin"

# ENV CC=clang
# ENV CXX=clang
ENV CMAKE_COLOR_DIAGNOSTICS=ON

ENTRYPOINT /x/compile.sh
