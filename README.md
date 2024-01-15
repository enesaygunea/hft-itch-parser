# hft-itch-parser

After copying the code to your local location, please follow the steps below.
  * docker build -t hftdev .
  * copy your test.pcap file into working directory.
  * docker run -it --name hftdevcon hftdev
  * ./hft-itch-parser pcapfile.pcap outputfile.txt
