CFLAGS = -O2 #-Wall  
GCC = gcc $(CFLAGS)
CORES2 = 0 1
CORES3x2 = 0 1 2 3 4 5
CORES3x3 = 0 1 2 3 4 5 6 7 8
CORES4x4 = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
CORES6x5 = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
CORES8x8 = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 \
37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
CORES16x8 = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 \
37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 \
77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 \
113 114 115 116 117 118 119 120 121 122 123 124 125 126 127
CORES16x16 = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 \
37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 \
77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 \
113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 \
143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 \
173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 \
203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 \
233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255

build: 
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=256 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64 -DBUS=1
noc_3x2:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=6 -DNOC_WIDTH=3 -DNOC_HEIGHT=2 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_3x3:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=9 -DNOC_WIDTH=3 -DNOC_HEIGHT=3 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_4x4:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=16 -DNOC_WIDTH=4 -DNOC_HEIGHT=4 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_6x5:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=30 -DNOC_WIDTH=6 -DNOC_HEIGHT=5 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_8x8:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=64 -DNOC_WIDTH=8 -DNOC_HEIGHT=8 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_16x8:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=128 -DNOC_WIDTH=16 -DNOC_HEIGHT=8 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64
noc_16x16:
	$(GCC) -o mpsoc_sim ./source/mpsoc_sim.c ./source/noc.c -lm -DN_CORES=256 -DNOC_WIDTH=16 -DNOC_HEIGHT=16 -DNOC_BUFFER_SIZE=16 -DOS_PACKET_SIZE=64

plots_single:
	cat ./reports/logout0.txt | grep "#" > ./reports/plot0.txt && \
	sed -i 's/#//g' ./reports/plot0.txt && \
	cat ./reports/logout0.txt | grep "!" > ./reports/sched0.txt && \
	sed -i 's/!//g' ./reports/sched0.txt \
	;done
	gnuplot ./source/plot_cpuload_single.txt
	gnuplot ./source/plot_tasksched_3x2_0.txt


plots_3x2two:
	for i in $(CORES2) ; do \
	echo "generating plots for core $$i.."; \
	cat ./reports/logout$$i.txt | grep "#" > ./reports/plot$$i.txt && \
	sed -i 's/#//g' ./reports/plot$$i.txt && \
	cat ./reports/logout$$i.txt | grep "!" > ./reports/sched$$i.txt && \
	sed -i 's/!//g' ./reports/sched$$i.txt \
	;done
	gnuplot ./source/plot_cpuload_3x2.txt
#	gnuplot ./source/plot_cputasks_3x2.txt
	gnuplot ./source/plot_netload_3x2.txt
#	gnuplot ./source/plot_tasksched_3x2_0.txt
#	gnuplot ./source/plot_tasksched_3x2_1.txt
#	gnuplot ./source/plot_tasksched_3x2_2.txt
#	gnuplot ./source/plot_tasksched_3x2_3.txt
#	gnuplot ./source/plot_tasksched_3x2_4.txt
#	gnuplot ./source/plot_tasksched_3x2_5.txt


plots_3x2:
	for i in $(CORES3x2) ; do \
	echo "generating plots for core $$i.."; \
	cat ./reports/logout$$i.txt | grep "#" > ./reports/plot$$i.txt && \
	sed -i 's/#//g' ./reports/plot$$i.txt && \
	cat ./reports/logout$$i.txt | grep "!" > ./reports/sched$$i.txt && \
	sed -i 's/!//g' ./reports/sched$$i.txt \
	;done
	gnuplot ./source/plot_cpuload_3x2.txt
#	gnuplot ./source/plot_cputasks_3x2.txt
	gnuplot ./source/plot_netload_3x2.txt
#	gnuplot ./source/plot_tasksched_3x2_0.txt
#	gnuplot ./source/plot_tasksched_3x2_1.txt
#	gnuplot ./source/plot_tasksched_3x2_2.txt
#	gnuplot ./source/plot_tasksched_3x2_3.txt
#	gnuplot ./source/plot_tasksched_3x2_4.txt
#	gnuplot ./source/plot_tasksched_3x2_5.txt


plots_4x4:
	for i in $(CORES4x4) ; do \
	echo "generating plots for core $$i.."; \
	cat ./reports/logout$$i.txt | grep "#" > ./reports/plot$$i.txt && \
	sed -i 's/#//g' ./reports/plot$$i.txt && \
	cat ./reports/logout$$i.txt | grep "!" > ./reports/sched$$i.txt && \
	sed -i 's/!//g' ./reports/sched$$i.txt \
	;done
	gnuplot ./source/plot_cpuload_4x4.txt
	gnuplot ./source/plot_netload_4x4.txt

plots_6x5:
	for i in $(CORES6x5) ; do \
	echo "generating plots for core $$i.."; \
	cat ./reports/logout$$i.txt | grep "#" > ./reports/plot$$i.txt && \
	sed -i 's/#//g' ./reports/plot$$i.txt && \
	cat ./reports/logout$$i.txt | grep "!" > ./reports/sched$$i.txt && \
	sed -i 's/!//g' ./reports/sched$$i.txt \
	;done
	gnuplot ./source/plot_cpuload_6x5.txt
	gnuplot ./source/plot_netload_6x5.txt

clean:
	-rm -rf ./reports/*.txt ./reports/*.eps ./reports/*.plt
	-rm -rf ./objects/*.bin
	-rm -rf ./source/*~
