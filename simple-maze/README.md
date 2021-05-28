# Simple-Maze
a simple code using Btree and dijkstra algorithm to find the lightest path:<br />
## input:
8 10<br />
0 0 0 1 0 0 0 0 0 0<br />
0 1 1 1 0 1 1 0 1 0<br />
0 1 0 1 0 0 1 1 1 0<br />
0 0 0 1 1 1 1 0 0 0<br />
0 1 1 1 0 0 1 1 1 0<br />
0 1 0 0 0 1 1 0 0 0<br />
0 1 0 1 1 0 1 1 1 0<br />
0 0 0 0 0 0 0 1 0 0<br />

## output:
result:<br />
<br />
  0  0  0 -1  0  0  0  0  0  0  <br />
  0  1  1 -1  0  1  1  0  1  0  <br />
  0  1  0 -1  0  0  1  1  1  0  <br />
  0  0  0 -1 -1 -1 -1  0  0  0  <br />
  0  1  1  1  0  0 -1  1  1  0  <br />
  0  1  0  0  0  1 -1  0  0  0  <br />
  0  1  0  1  1  0 -1 -1  1  0  <br />
  0  0  0  0  0  0  0 -1  0  0  <br />
