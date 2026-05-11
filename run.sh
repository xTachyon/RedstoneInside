set -ex

podman build .
id=$(podman build . -q)

podman run --rm -v .:/x:z --pull never $id
