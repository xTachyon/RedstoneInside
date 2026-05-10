FROM ubuntu:26.04

RUN apt update \
    && apt install -y cmake ninja-build clang build-essential \
    gcc g++ \
    libboost-all-dev
RUN apt install nlohmann-json3-dev

# ENV CC=clang
# ENV CXX=clang
ENV CMAKE_COLOR_DIAGNOSTICS=ON

ENTRYPOINT /x/compile.sh
