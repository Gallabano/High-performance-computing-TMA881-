__kernel void computeNewTemp(__global const double * prev_temp, __global double * next_temp, __private double conductivity, __private int width, __private int height)
{
  int i = get_global_id(0);
  int j = get_global_id(1);

  double map, west, east, south, north;
  map = prev_temp[ i * width + j ];
  west = ( j-1 >= 0 ? prev_temp[i*width + j-1] : 0);
  east = ( j+1 < width ? prev_temp[i*width + j+1] : 0);
  south = ( i+1 < height ? prev_temp[(i+1)*width + j] : 0);
  north = ( i-1 >= 0 ? prev_temp[(i-1)*width + j] : 0);

  next_temp[ i * width + j ] = map + conductivity * ( 0.25 * ( west + east + south + north ) - map );
}


__kernel
void
reduction(
  __global double *next_temp,
  __local double *scratch,
  __const int sz,
  __global double *result
  )
{
  int gsz = get_global_size(0);
  int gix = get_global_id(0);
  int lsz = get_local_size(0);
  int lix = get_local_id(0);

  double acc = 0;
  for ( int cix = get_global_id(0); cix < sz; cix += gsz )
    acc += next_temp[cix];

  scratch[lix] = acc;
  barrier(CLK_LOCAL_MEM_FENCE);

  for(int offset = lsz/2; offset > 0; offset /= 2) {
    if ( lix < offset )
      scratch[lix] += scratch[lix+offset];
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if ( lix == 0 )
    result[get_group_id(0)] = scratch[0];
}