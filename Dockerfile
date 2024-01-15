FROM ubuntu:22.04 as base

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    gdb \
    python3-pip

RUN pip3 install conan

WORKDIR /usr/src/hft-itch-demo

COPY . .

RUN conan profile detect --force
RUN conan install . -s build_type=Debug --output-folder=build --build=missing

WORKDIR /usr/src/hft-itch-demo/build
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
RUN cmake --build .

CMD ["bash"]