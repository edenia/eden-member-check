cd contracts
docker build --platform linux/x86_64 -t edenmembercheck .
docker run -d --name edencontracts edenmembercheck
docker cp edencontracts:/root/work/edenmember/build/edenmember.abi ../build
docker cp edencontracts:/root/work/edenmember/build/edenmember.wasm ../build
docker stop edencontracts
docker rm edencontracts
