# hft-itch-parser

After copying the code to your local location, please follow the steps below.
  * docker build -t hftdev .
  * copy your test.pcap file into working directory.
  * docker run -it --name hftdevcon hftdev
  * ./hft-itch-parser pcapfile.pcap outputfile.txt

Sample output is;
![image](https://github.com/enesaygunea/hft-itch-parser/assets/76591748/349c07d0-2d11-4073-8f50-e0764984095f)
