FROM ubuntu:26.04 as base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build zlib1g-dev \
    clang build-essential \
    wget python3 python3-dev git


# ENV CC=clang
# ENV CXX=clang


# -----------------------------------------------------------------------------
    
FROM base_image as download_boost
WORKDIR /b
RUN wget https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.bz2

# -----------------------------------------------------------------------------

FROM base_image as build_boost

WORKDIR /b
COPY --from=download_boost /b /b
RUN tar -xjf boost_1_85_0.tar.bz2
WORKDIR /b/boost_1_85_0
RUN ./bootstrap.sh --prefix=/b/boost/
RUN ./b2 install -j $(nproc) --with-system --with-filesystem --with-iostreams --with-program_options

# -----------------------------------------------------------------------------

FROM base_image as clone_json

WORKDIR /libs
RUN git clone https://github.com/nlohmann/json.git
RUN cd json && git checkout v3.12.0

# -----------------------------------------------------------------------------

FROM base_image

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y ninja-build build-essential cmake clang
RUN apt install -y wget
RUN apt install -y python3 python3-dev git

COPY --from=build_boost /b/boost /libs/boost
COPY --from=clone_json /libs/json /libs/json

ENV CMAKE_COLOR_DIAGNOSTICS=ON

ENTRYPOINT /x/compile.sh
