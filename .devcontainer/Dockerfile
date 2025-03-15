FROM ubuntu:22.04

# ตั้งค่าตัวแปรลดการโต้ตอบของ apt
ENV DEBIAN_FRONTEND=noninteractive

# ติดตั้งเครื่องมือพื้นฐาน
# RUN apt-get update && apt-get install -y \
#     build-essential cmake gdb gdbserver \
#     libopencv-dev


# ติดตั้งเครื่องมือพัฒนา C++ และ OpenCV dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake gdb gdbserver git pkg-config \
    libgtk-3-dev libcanberra-gtk3-module \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libopencv-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev libtbb2 libtbb-dev libdc1394-dev \
    python3-dev python3-numpy python3-pip \
    ffmpeg && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# ติดตั้ง OpenCV 4.5.5
RUN git clone -b 4.5.5 https://github.com/opencv/opencv.git && \
    git clone -b 4.5.5 https://github.com/opencv/opencv_contrib.git && \
    cd opencv && mkdir build && cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
          -D WITH_TBB=ON \
          -D WITH_V4L=ON \
          -D OPENCV_GENERATE_PKGCONFIG=ON \
          -D WITH_OPENMP=ON \
          -D WITH_FFMPEG=ON \
          -D BUILD_EXAMPLES=OFF .. && \
    make -j"$(nproc)" && \
    make install && \
    ldconfig && \
    rm -rf /opencv /opencv_contrib

# ตั้งค่า user (optional)
# RUN useradd -m dev && echo "dev:dev" | chpasswd && adduser dev sudo
# USER dev

# กำหนดโฟลเดอร์ทำงาน
WORKDIR /workspace
