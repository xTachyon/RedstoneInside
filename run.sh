set -ex

if [ -z "id" ]; then
    podman build .
    id=$(podman build . -q)
fi

podman run --rm -v .:/x:z --pull never $id
