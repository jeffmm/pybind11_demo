FROM manifoldai/orbyter-ml-dev:3.5.4
ADD requirements.txt /build/requirements.txt
WORKDIR /build/
RUN apt update && apt install -y pybind11-dev && \
    rm -rf /var/lib/apt/lists/*
RUN pip install -r requirements.txt
WORKDIR /mnt/
