/*
 *	MP3 window subband -> subband filtering -> mdct routine
 *
 *	Copyright (c) 1999 Takehiro TOMINAGA
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 *         Special Thanks to Patrick De Smet for your advices.
 */


#include "util.h"
#include "l3side.h"
#include "newmdct.h"

#define SCALE (32768.0/ 2.384e-06)

#ifndef USE_GOGO_SUBBAND
static const FLOAT8 enwindow[] = 
{
      -4.77e-07*0.740951125354959/2.384e-06,  1.03951e-04*0.740951125354959/2.384e-06,  9.53674e-04*0.740951125354959/2.384e-06, 2.841473e-03*0.740951125354959/2.384e-06,
  3.5758972e-02*0.740951125354959/2.384e-06, 3.401756e-03*0.740951125354959/2.384e-06,  9.83715e-04*0.740951125354959/2.384e-06,   9.9182e-05*0.740951125354959/2.384e-06, /* 15*/
     1.2398e-05*0.740951125354959/2.384e-06,  1.91212e-04*0.740951125354959/2.384e-06, 2.283096e-03*0.740951125354959/2.384e-06,1.6994476e-02*0.740951125354959/2.384e-06,
 -1.8756866e-02*0.740951125354959/2.384e-06,-2.630711e-03*0.740951125354959/2.384e-06, -2.47478e-04*0.740951125354959/2.384e-06,  -1.4782e-05*0.740951125354959/2.384e-06,

      -4.77e-07*0.773010453362737/2.384e-06,  1.05858e-04*0.773010453362737/2.384e-06,  9.30786e-04*0.773010453362737/2.384e-06, 2.521515e-03*0.773010453362737/2.384e-06,
  3.5694122e-02*0.773010453362737/2.384e-06, 3.643036e-03*0.773010453362737/2.384e-06,  9.91821e-04*0.773010453362737/2.384e-06,   9.6321e-05*0.773010453362737/2.384e-06, /* 14*/
     1.1444e-05*0.773010453362737/2.384e-06,  1.65462e-04*0.773010453362737/2.384e-06, 2.110004e-03*0.773010453362737/2.384e-06,1.6112804e-02*0.773010453362737/2.384e-06,
 -1.9634247e-02*0.773010453362737/2.384e-06,-2.803326e-03*0.773010453362737/2.384e-06, -2.77042e-04*0.773010453362737/2.384e-06,  -1.6689e-05*0.773010453362737/2.384e-06,

      -4.77e-07*0.803207531480645/2.384e-06,  1.07288e-04*0.803207531480645/2.384e-06,  9.02653e-04*0.803207531480645/2.384e-06, 2.174854e-03*0.803207531480645/2.384e-06,
  3.5586357e-02*0.803207531480645/2.384e-06, 3.858566e-03*0.803207531480645/2.384e-06,  9.95159e-04*0.803207531480645/2.384e-06,   9.3460e-05*0.803207531480645/2.384e-06, /* 13*/
     1.0014e-05*0.803207531480645/2.384e-06,  1.40190e-04*0.803207531480645/2.384e-06, 1.937389e-03*0.803207531480645/2.384e-06,1.5233517e-02*0.803207531480645/2.384e-06,
 -2.0506859e-02*0.803207531480645/2.384e-06,-2.974033e-03*0.803207531480645/2.384e-06, -3.07560e-04*0.803207531480645/2.384e-06,  -1.8120e-05*0.803207531480645/2.384e-06,

      -4.77e-07*0.831469612302545/2.384e-06,  1.08242e-04*0.831469612302545/2.384e-06,  8.68797e-04*0.831469612302545/2.384e-06, 1.800537e-03*0.831469612302545/2.384e-06,
  3.5435200e-02*0.831469612302545/2.384e-06, 4.049301e-03*0.831469612302545/2.384e-06,  9.94205e-04*0.831469612302545/2.384e-06,   9.0599e-05*0.831469612302545/2.384e-06, /* 12*/
      9.060e-06*0.831469612302545/2.384e-06,  1.16348e-04*0.831469612302545/2.384e-06, 1.766682e-03*0.831469612302545/2.384e-06,1.4358521e-02*0.831469612302545/2.384e-06,
 -2.1372318e-02*0.831469612302545/2.384e-06, -3.14188e-03*0.831469612302545/2.384e-06, -3.39031e-04*0.831469612302545/2.384e-06,  -1.9550e-05*0.831469612302545/2.384e-06,

      -4.77e-07*0.857728610000272/2.384e-06,  1.08719e-04*0.857728610000272/2.384e-06,  8.29220e-04*0.857728610000272/2.384e-06, 1.399517e-03*0.857728610000272/2.384e-06,
  3.5242081e-02*0.857728610000272/2.384e-06, 4.215240e-03*0.857728610000272/2.384e-06,  9.89437e-04*0.857728610000272/2.384e-06,   8.7261e-05*0.857728610000272/2.384e-06, /* 11*/
      8.106e-06*0.857728610000272/2.384e-06,   9.3937e-05*0.857728610000272/2.384e-06, 1.597881e-03*0.857728610000272/2.384e-06,1.3489246e-02*0.857728610000272/2.384e-06,
 -2.2228718e-02*0.857728610000272/2.384e-06,-3.306866e-03*0.857728610000272/2.384e-06, -3.71456e-04*0.857728610000272/2.384e-06,  -2.1458e-05*0.857728610000272/2.384e-06,

      -4.77e-07*0.881921264348355/2.384e-06,  1.08719e-04*0.881921264348355/2.384e-06,   7.8392e-04*0.881921264348355/2.384e-06,  9.71317e-04*0.881921264348355/2.384e-06,
  3.5007000e-02*0.881921264348355/2.384e-06, 4.357815e-03*0.881921264348355/2.384e-06,  9.80854e-04*0.881921264348355/2.384e-06,   8.3923e-05*0.881921264348355/2.384e-06, /* 10*/
      7.629e-06*0.881921264348355/2.384e-06,   7.2956e-05*0.881921264348355/2.384e-06, 1.432419e-03*0.881921264348355/2.384e-06,1.2627602e-02*0.881921264348355/2.384e-06,
 -2.3074150e-02*0.881921264348355/2.384e-06,-3.467083e-03*0.881921264348355/2.384e-06, -4.04358e-04*0.881921264348355/2.384e-06,  -2.3365e-05*0.881921264348355/2.384e-06,

      -9.54e-07*0.903989293123443/2.384e-06,  1.08242e-04*0.903989293123443/2.384e-06,  7.31945e-04*0.903989293123443/2.384e-06,  5.15938e-04*0.903989293123443/2.384e-06,
  3.4730434e-02*0.903989293123443/2.384e-06, 4.477024e-03*0.903989293123443/2.384e-06,  9.68933e-04*0.903989293123443/2.384e-06,   8.0585e-05*0.903989293123443/2.384e-06, /* 9*/
      6.676e-06*0.903989293123443/2.384e-06,   5.2929e-05*0.903989293123443/2.384e-06, 1.269817e-03*0.903989293123443/2.384e-06,1.1775017e-02*0.903989293123443/2.384e-06,
 -2.3907185e-02*0.903989293123443/2.384e-06,-3.622532e-03*0.903989293123443/2.384e-06, -4.38213e-04*0.903989293123443/2.384e-06,  -2.5272e-05*0.903989293123443/2.384e-06,

      -9.54e-07*0.92387953251128675613/2.384e-06,  1.06812e-04*0.92387953251128675613/2.384e-06,  6.74248e-04*0.92387953251128675613/2.384e-06,   3.3379e-05*0.92387953251128675613/2.384e-06,
  3.4412861e-02*0.92387953251128675613/2.384e-06, 4.573822e-03*0.92387953251128675613/2.384e-06,  9.54151e-04*0.92387953251128675613/2.384e-06,   7.6771e-05*0.92387953251128675613/2.384e-06,
      6.199e-06*0.92387953251128675613/2.384e-06,   3.4332e-05*0.92387953251128675613/2.384e-06, 1.111031e-03*0.92387953251128675613/2.384e-06,1.0933399e-02*0.92387953251128675613/2.384e-06,
 -2.4725437e-02*0.92387953251128675613/2.384e-06,-3.771782e-03*0.92387953251128675613/2.384e-06, -4.72546e-04*0.92387953251128675613/2.384e-06,  -2.7657e-05*0.92387953251128675613/2.384e-06,

      -9.54e-07*0.941544065183021/2.384e-06,  1.05381e-04*0.941544065183021/2.384e-06,  6.10352e-04*0.941544065183021/2.384e-06, -4.75883e-04*0.941544065183021/2.384e-06,
  3.4055710e-02*0.941544065183021/2.384e-06, 4.649162e-03*0.941544065183021/2.384e-06,  9.35555e-04*0.941544065183021/2.384e-06,   7.3433e-05*0.941544065183021/2.384e-06, /* 7*/
      5.245e-06*0.941544065183021/2.384e-06,   1.7166e-05*0.941544065183021/2.384e-06,  9.56535e-04*0.941544065183021/2.384e-06,1.0103703e-02*0.941544065183021/2.384e-06,
 -2.5527000e-02*0.941544065183021/2.384e-06,-3.914356e-03*0.941544065183021/2.384e-06, -5.07355e-04*0.941544065183021/2.384e-06,  -3.0041e-05*0.941544065183021/2.384e-06,

      -9.54e-07*0.956940335732209/2.384e-06,  1.02520e-04*0.956940335732209/2.384e-06,  5.39303e-04*0.956940335732209/2.384e-06,-1.011848e-03*0.956940335732209/2.384e-06,
  3.3659935e-02*0.956940335732209/2.384e-06, 4.703045e-03*0.956940335732209/2.384e-06,  9.15051e-04*0.956940335732209/2.384e-06,   7.0095e-05*0.956940335732209/2.384e-06, /* 6*/
      4.768e-06*0.956940335732209/2.384e-06,     9.54e-07*0.956940335732209/2.384e-06,  8.06808e-04*0.956940335732209/2.384e-06, 9.287834e-03*0.956940335732209/2.384e-06,
 -2.6310921e-02*0.956940335732209/2.384e-06,-4.048824e-03*0.956940335732209/2.384e-06, -5.42164e-04*0.956940335732209/2.384e-06,  -3.2425e-05*0.956940335732209/2.384e-06,

     -1.431e-06*0.970031253194544/2.384e-06,   9.9182e-05*0.970031253194544/2.384e-06,  4.62532e-04*0.970031253194544/2.384e-06,-1.573563e-03*0.970031253194544/2.384e-06,
  3.3225536e-02*0.970031253194544/2.384e-06, 4.737377e-03*0.970031253194544/2.384e-06,  8.91685e-04*0.970031253194544/2.384e-06,   6.6280e-05*0.970031253194544/2.384e-06,  /* 5*/
      4.292e-06*0.970031253194544/2.384e-06,  -1.3828e-05*0.970031253194544/2.384e-06,  6.61850e-04*0.970031253194544/2.384e-06, 8.487225e-03*0.970031253194544/2.384e-06,
 -2.7073860e-02*0.970031253194544/2.384e-06,-4.174709e-03*0.970031253194544/2.384e-06, -5.76973e-04*0.970031253194544/2.384e-06,  -3.4809e-05*0.970031253194544/2.384e-06,

     -1.431e-06*0.98078528040323/2.384e-06,   9.5367e-05*0.98078528040323/2.384e-06,  3.78609e-04*0.98078528040323/2.384e-06,-2.161503e-03*0.98078528040323/2.384e-06,
  3.2754898e-02*0.98078528040323/2.384e-06, 4.752159e-03*0.98078528040323/2.384e-06,  8.66413e-04*0.98078528040323/2.384e-06,   6.2943e-05*0.98078528040323/2.384e-06, /* 4*/
      3.815e-06*0.98078528040323/2.384e-06,   -2.718e-05*0.98078528040323/2.384e-06,  5.22137e-04*0.98078528040323/2.384e-06, 7.703304e-03*0.98078528040323/2.384e-06,
 -2.7815342e-02*0.98078528040323/2.384e-06,-4.290581e-03*0.98078528040323/2.384e-06, -6.11782e-04*0.98078528040323/2.384e-06,  -3.7670e-05*0.98078528040323/2.384e-06,

     -1.907e-06*0.989176509964781/2.384e-06,   9.0122e-05*0.989176509964781/2.384e-06,  2.88486e-04*0.989176509964781/2.384e-06,-2.774239e-03*0.989176509964781/2.384e-06,
  3.2248020e-02*0.989176509964781/2.384e-06, 4.748821e-03*0.989176509964781/2.384e-06,  8.38757e-04*0.989176509964781/2.384e-06,   5.9605e-05*0.989176509964781/2.384e-06, /* 3*/
      3.338e-06*0.989176509964781/2.384e-06,  -3.9577e-05*0.989176509964781/2.384e-06,  3.88145e-04*0.989176509964781/2.384e-06, 6.937027e-03*0.989176509964781/2.384e-06,
 -2.8532982e-02*0.989176509964781/2.384e-06,-4.395962e-03*0.989176509964781/2.384e-06, -6.46591e-04*0.989176509964781/2.384e-06,  -4.0531e-05*0.989176509964781/2.384e-06,

     -1.907e-06*0.995184726672197/2.384e-06,   8.4400e-05*0.995184726672197/2.384e-06,  1.91689e-04*0.995184726672197/2.384e-06,-3.411293e-03*0.995184726672197/2.384e-06,
  3.1706810e-02*0.995184726672197/2.384e-06, 4.728317e-03*0.995184726672197/2.384e-06,  8.09669e-04*0.995184726672197/2.384e-06,    5.579e-05*0.995184726672197/2.384e-06,
      3.338e-06*0.995184726672197/2.384e-06,  -5.0545e-05*0.995184726672197/2.384e-06,  2.59876e-04*0.995184726672197/2.384e-06, 6.189346e-03*0.995184726672197/2.384e-06,
 -2.9224873e-02*0.995184726672197/2.384e-06,-4.489899e-03*0.995184726672197/2.384e-06, -6.80923e-04*0.995184726672197/2.384e-06,  -4.3392e-05*0.995184726672197/2.384e-06,

     -2.384e-06*0.998795456205172/2.384e-06,   7.7724e-05*0.998795456205172/2.384e-06,   8.8215e-05*0.998795456205172/2.384e-06,-4.072189e-03*0.998795456205172/2.384e-06,
      3.1132698e-02*0.998795456205172/2.384e-06, 4.691124e-03*0.998795456205172/2.384e-06,  7.79152e-04*0.998795456205172/2.384e-06,   5.2929e-05*0.998795456205172/2.384e-06,
      2.861e-06*0.998795456205172/2.384e-06,  -6.0558e-05*0.998795456205172/2.384e-06,  1.37329e-04*0.998795456205172/2.384e-06, 5.462170e-03*0.998795456205172/2.384e-06,
      -2.9890060e-02*0.998795456205172/2.384e-06,-4.570484e-03*0.998795456205172/2.384e-06, -7.14302e-04*0.998795456205172/2.384e-06,  -4.6253e-05*0.998795456205172/2.384e-06,

      3.5780907e-02 * SQRT2*0.5/2.384e-06,1.7876148e-02 * SQRT2*0.5/2.384e-06, 3.134727e-03 * SQRT2*0.5/2.384e-06, 2.457142e-03 * SQRT2*0.5/2.384e-06,
      9.71317e-04 * SQRT2*0.5/2.384e-06,  2.18868e-04 * SQRT2*0.5/2.384e-06,  1.01566e-04 * SQRT2*0.5/2.384e-06,   1.3828e-05 * SQRT2*0.5/2.384e-06,

      3.0526638e-02/2.384e-06, 4.638195e-03/2.384e-06,  7.47204e-04/2.384e-06,   4.9591e-05/2.384e-06,
      4.756451e-03/2.384e-06,   2.1458e-05/2.384e-06,  -6.9618e-05/2.384e-06,/*    2.384e-06/2.384e-06*/

      1.960342806591213e-01,
      3.901806440322567e-01,
      5.805693545089249e-01,
      7.653668647301797e-01,
      9.427934736519954e-01,
      1.111140466039205e+00,
      1.268786568327291e+00,
      1.414213562373095e+00,
      1.546020906725474e+00,
      1.662939224605090e+00,
      1.763842528696710e+00,
      1.847759065022573e+00,
      1.913880671464418e+00,
      1.961570560806461e+00,
      1.990369453344394e+00,

      9.063471690191471e-01,
      8.206787908286602e-01,
      7.416505462720353e-01,
      6.681786379192989e-01,
      5.993769336819237e-01,
      5.345111359507916e-01,
      4.729647758913199e-01,

      0.41421356237309504879,  /* tan(PI/8) */

      3.578057213145241e-01,
      3.033466836073424e-01,
      2.504869601913055e-01,
      1.989123673796580e-01,
      1.483359875383474e-01,
      9.849140335716425e-02,
      4.912684976946725e-02,
};
#endif


#define NS 12
#define NL 36

static const FLOAT8 win[4][NL] = {
  {
    2.382191739347913e-13,
    6.423305872147834e-13,
    9.400849094049688e-13,
    1.122435026096556e-12,
    1.183840321267481e-12,
    1.122435026096556e-12,
    9.400849094049690e-13,
    6.423305872147839e-13,
    2.382191739347918e-13,
    5.456116108943412e-12,
    4.878985199565852e-12,
    4.240448995017367e-12,
    3.559909094758252e-12,
    2.858043359288075e-12,
    2.156177623817898e-12,
    1.475637723558783e-12,
    8.371015190102974e-13,
    2.599706096327376e-13,
    -5.456116108943412e-12,
    -4.878985199565852e-12,
    -4.240448995017367e-12,
    -3.559909094758252e-12,
    -2.858043359288076e-12,
    -2.156177623817898e-12,
    -1.475637723558783e-12,
    -8.371015190102975e-13,
    -2.599706096327376e-13,
    -2.382191739347923e-13,
    -6.423305872147843e-13,
    -9.400849094049696e-13,
    -1.122435026096556e-12,
    -1.183840321267481e-12,
    -1.122435026096556e-12,
    -9.400849094049694e-13,
    -6.423305872147840e-13,
    -2.382191739347918e-13,
  },
  {
    2.382191739347913e-13,
    6.423305872147834e-13,
    9.400849094049688e-13,
    1.122435026096556e-12,
    1.183840321267481e-12,
    1.122435026096556e-12,
    9.400849094049688e-13,
    6.423305872147841e-13,
    2.382191739347918e-13,
    5.456116108943413e-12,
    4.878985199565852e-12,
    4.240448995017367e-12,
    3.559909094758253e-12,
    2.858043359288075e-12,
    2.156177623817898e-12,
    1.475637723558782e-12,
    8.371015190102975e-13,
    2.599706096327376e-13,
    -5.461314069809755e-12,
    -4.921085770524055e-12,
    -4.343405037091838e-12,
    -3.732668368707687e-12,
    -3.093523840190885e-12,
    -2.430835727329465e-12,
    -1.734679010007751e-12,
    -9.748253656609281e-13,
    -2.797435120168326e-13,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -2.283748241799531e-13,
    -4.037858874020686e-13,
    -2.146547464825323e-13,
  },
  {
    1.316524975873958e-01, /* win[SHORT_TYPE] */
    4.142135623730950e-01,
    7.673269879789602e-01,

    1.091308501069271e+00, /* tantab_l */
    1.303225372841206e+00,
    1.569685577117490e+00,
    1.920982126971166e+00,
    2.414213562373094e+00,
    3.171594802363212e+00,
    4.510708503662055e+00,
    7.595754112725146e+00,
    2.290376554843115e+01,

    0.98480775301220802032, /* cx */
    0.64278760968653936292,
    0.34202014332566882393,
    0.93969262078590842791,
    -0.17364817766693030343,
    -0.76604444311897790243,
    0.86602540378443870761,
    0.500000000000000e+00,

    -5.144957554275265e-01, /* ca */
    -4.717319685649723e-01,
    -3.133774542039019e-01,
    -1.819131996109812e-01,
    -9.457419252642064e-02,
    -4.096558288530405e-02,
    -1.419856857247115e-02,
    -3.699974673760037e-03,

     8.574929257125442e-01, /* cs */
     8.817419973177052e-01,
     9.496286491027329e-01,
     9.833145924917901e-01,
     9.955178160675857e-01,
     9.991605581781475e-01,
     9.998991952444470e-01,
     9.999931550702802e-01,
  },
  {
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.283748241799531e-13,
    4.037858874020686e-13,
    2.146547464825323e-13,
    5.461314069809755e-12,
    4.921085770524055e-12,
    4.343405037091838e-12,
    3.732668368707687e-12,
    3.093523840190885e-12,
    2.430835727329466e-12,
    1.734679010007751e-12,
    9.748253656609281e-13,
    2.797435120168326e-13,
    -5.456116108943413e-12,
    -4.878985199565852e-12,
    -4.240448995017367e-12,
    -3.559909094758253e-12,
    -2.858043359288075e-12,
    -2.156177623817898e-12,
    -1.475637723558782e-12,
    -8.371015190102975e-13,
    -2.599706096327376e-13,
    -2.382191739347913e-13,
    -6.423305872147834e-13,
    -9.400849094049688e-13,
    -1.122435026096556e-12,
    -1.183840321267481e-12,
    -1.122435026096556e-12,
    -9.400849094049688e-13,
    -6.423305872147841e-13,
    -2.382191739347918e-13,
  }
};

#define tantab_l (win[SHORT_TYPE]+3)
#define cx (win[SHORT_TYPE]+12)
#define ca (win[SHORT_TYPE]+20)
#define cs (win[SHORT_TYPE]+28)

/************************************************************************
*
* window_subband()
*
* PURPOSE:  Overlapping window on PCM samples
*
* SEMANTICS:
* 32 16-bit pcm samples are scaled to fractional 2's complement and
* concatenated to the end of the window buffer #x#. The updated window
* buffer #x# is then windowed by the analysis window #c# to produce the
* windowed sample #z#
*
************************************************************************/

/*
 *      new IDCT routine written by Takehiro TOMINAGA
 */
static const int order[] = {
  0, 1,12,17, 8, 9, 4,25,24, 5,20,21,16,13,28,29,
 26, 3,18,19,10,11, 2,27,22, 7, 6,23,15,14,30,31
};


/* returns sum_j=0^31 a[j]*cos(PI*j*(k+1/2)/32), 0<=k<32 */
INLINE static void window_subband(sample_t *x1, FLOAT8 a[SBLIMIT])
{
    int i;
    FLOAT8 const *wp = enwindow;

    sample_t *x2 = &x1[238-14-286];

    for (i = -15; i < 0; i++) {
	FLOAT8 w, s, t;

	w = wp[ 0]; s  = x2[-224] * w; t  = x1[ 224] * w;
	w = wp[ 1]; s += x2[-160] * w; t += x1[ 160] * w;
	w = wp[ 2]; s += x2[- 96] * w; t += x1[  96] * w;
	w = wp[ 3]; s += x2[- 32] * w; t += x1[  32] * w;
	w = wp[ 4]; s += x2[  32] * w; t += x1[- 32] * w;
	w = wp[ 5]; s += x2[  96] * w; t += x1[- 96] * w;
	w = wp[ 6]; s += x2[ 160] * w; t += x1[-160] * w;
	w = wp[ 7]; s += x2[ 224] * w; t += x1[-224] * w;

	w = wp[ 8]; s += x1[-256] * w; t -= x2[ 256] * w;
	w = wp[ 9]; s += x1[-192] * w; t -= x2[ 192] * w;
	w = wp[10]; s += x1[-128] * w; t -= x2[ 128] * w;
	w = wp[11]; s += x1[- 64] * w; t -= x2[  64] * w;
	w = wp[12]; s += x1[   0] * w; t -= x2[   0] * w;
	w = wp[13]; s += x1[  64] * w; t -= x2[- 64] * w;
	w = wp[14]; s += x1[ 128] * w; t -= x2[-128] * w;
	w = wp[15]; s += x1[ 192] * w; t -= x2[-192] * w;
	wp += 16;

	a[0] = t; // old 15,14,13,...
	a[1] = s; // old 17,18,19,...
	a+=2;
	x1--;
	x2++;
    }
    {
	FLOAT8 s,t,u,v;
	t  =  x1[- 16] * wp[0];               s  = x1[ -32] * wp[ 8];
	t += (x1[- 48] - x1[ 16]) * wp[1];    s += x1[ -96] * wp[ 9];
	t += (x1[- 80] + x1[ 48]) * wp[2];    s += x1[-160] * wp[10];
	t += (x1[-112] - x1[ 80]) * wp[3];    s += x1[-224] * wp[11];
	t += (x1[-144] + x1[112]) * wp[4];    s -= x1[  32] * wp[12];
	t += (x1[-176] - x1[144]) * wp[5];    s -= x1[  96] * wp[13];
	t += (x1[-208] + x1[176]) * wp[6];    s -= x1[ 160] * wp[14];
	t += (x1[-240] - x1[208]) * wp[7];    s -= x1[ 224];

	u = s - t;
	v = s + t;

	t = a[14-30] + a[15-30] * wp[30+7];
	s = a[15-30] - a[14-30] * wp[30+7];

	a[15-30] = v - t;   // A3
	a[14-30] = u + s;   // A2
	a[31-30] = v + t;   // A0
	a[30-30] = u - s;   // A1
    }
    for (i = 0; i < 7; i++) {
	FLOAT8 s, t, u, v;
	u = a[i*2-29] * wp[i+30];
	v = a[i*2-13] * wp[i+38];

	s = a[i*2-30] - u;
	t = a[i*2-14] - v;

	a[i*2-30] += u;
	a[i*2-14] += v;

	a[i*2-29] = wp[15+i] * s;
	a[i*2-13] = wp[23+i] * t;
    }
{
    FLOAT8 xr;
    xr = a[26-30] - a[ 2-30]; a[26-30] += a[ 2-30]; a[ 2-30] = xr * wp[15+11];
    xr = a[ 3-30] - a[27-30]; a[ 3-30] += a[27-30]; a[27-30] = xr * wp[15+11];
    xr = a[18-30] - a[10-30]; a[10-30] += a[18-30]; a[18-30] = xr * wp[15+3];
    xr = a[19-30] - a[11-30]; a[11-30] += a[19-30]; a[19-30] = xr * wp[15+3];

    xr = a[28-30] - a[ 0-30]; a[ 0-30] += a[28-30]; a[28-30] = wp[15+13] * xr;
    xr =-a[ 1-30] + a[29-30]; a[ 1-30] += a[29-30]; a[29-30] = wp[15+13] * xr;

    xr = a[20-30] - a[ 8-30]; a[ 8-30] += a[20-30]; a[20-30] = wp[15+5] * xr;
    xr = a[ 9-30] - a[21-30]; a[ 9-30] += a[21-30]; a[21-30] = wp[15+5] * xr;

    xr = a[24-30] - a[ 4-30]; a[24-30] += a[ 4-30]; a[ 4-30] = wp[15+9] * xr;
    xr = a[ 5-30] - a[25-30]; a[ 5-30] += a[25-30]; a[25-30] = wp[15+9] * xr;

    xr = a[16-30] - a[12-30]; a[16-30] += a[12-30]; a[12-30] = wp[15+1] * xr;
    xr = a[17-30] - a[13-30]; a[13-30] += a[17-30]; a[17-30] = wp[15+1] * xr;

    xr = a[ 0-30] - a[16-30]; a[ 0-30] += a[16-30]; a[16-30] = wp[15+11] * xr;
    xr = a[ 1-30] - a[13-30]; a[ 1-30] += a[13-30]; a[13-30] = wp[15+11] * xr;
    xr = a[12-30] - a[28-30]; a[12-30] += a[28-30]; a[28-30] = wp[15+11] * xr;
    xr =-a[17-30] + a[29-30]; a[17-30] += a[29-30]; a[29-30] = wp[15+11] * xr;

    xr = a[24-30] - a[ 8-30]; a[24-30] += a[ 8-30]; a[ 8-30] = wp[15+3] * xr;
    xr = a[ 5-30] - a[ 9-30]; a[ 5-30] += a[ 9-30]; a[ 9-30] = wp[15+3] * xr;
    xr =-a[20-30] + a[ 4-30]; a[20-30] += a[ 4-30]; a[ 4-30] = wp[15+3] * xr;
    xr = a[21-30] - a[25-30]; a[21-30] += a[25-30]; a[25-30] = wp[15+3] * xr;


    xr = a[22-30] - a[ 6-30]; a[22-30] += a[ 6-30]; a[ 6-30] = xr * SQRT2;
    xr =-a[ 7-30] + a[23-30]; a[ 7-30] += a[23-30]; a[23-30] = xr * SQRT2 - a[ 7-30];

    a[ 7-30] -= a[22-30];
    a[ 6-30] -= a[ 7-30];
    a[23-30] -= a[ 6-30];

    xr = a[22-30]; a[22-30] = a[31-30] - xr; a[31-30] = a[31-30] + xr;
    xr = a[ 6-30]; a[ 6-30] = a[14-30] - xr; a[14-30] = a[14-30] + xr;
    xr = a[ 7-30]; a[ 7-30] = a[30-30] - xr; a[30-30] = a[30-30] + xr;
    xr = a[23-30]; a[23-30] = a[15-30] - xr; a[15-30] = a[15-30] + xr;

    xr = SQRT2 * (a[26-30] - a[10-30]); a[26-30] += a[10-30]; a[10-30] = xr;
    xr = SQRT2 * (a[ 3-30] - a[11-30]); a[ 3-30] += a[11-30]; a[11-30] = xr;
    xr =-SQRT2 * (a[18-30] - a[ 2-30]); a[18-30] += a[ 2-30]; a[ 2-30] = xr - a[18-30];
    xr =-SQRT2 * (a[19-30] + a[27-30]); a[19-30] -= a[27-30]; a[27-30] = xr - a[19-30];

    xr = a[26-30];
    a[19-30] -= a[ 3-30];
    a[ 3-30] -= xr;
    a[26-30] = a[31-30] - xr;
    a[31-30] += xr;

    xr = a[ 3-30];
    a[11-30] -= a[19-30];
    a[18-30] -= xr;
    a[ 3-30] = a[30-30] - xr;
    a[30-30] += xr;

    xr = a[18-30];
    a[27-30] -= a[11-30];
    a[19-30] -= xr;
    a[18-30] = a[14-30] - xr;
    a[14-30] += xr;

    xr = a[19-30];
    a[10-30] -= xr;
    a[19-30] = a[15-30] - xr;
    a[15-30] += xr;

    xr = a[10-30];
    a[11-30] -= xr;
    a[10-30] = a[23-30] - xr;
    a[23-30] += xr;

    xr = a[11-30];
    a[ 2-30] -= xr;
    a[11-30] = a[ 6-30] - xr;
    a[ 6-30] += xr;

    xr = a[ 2-30];
    a[27-30] -= xr;
    a[ 2-30] = a[ 7-30] - xr; a[ 7-30] += xr;
    xr = a[27-30]; a[27-30] = a[22-30] - xr; a[22-30] += xr;

    xr = SQRT2 * (a[ 0-30] - a[24-30]); a[ 0-30] += a[24-30]; a[24-30] = xr;
    xr = SQRT2 * (a[ 1-30] - a[ 5-30]); a[ 1-30] += a[ 5-30]; a[ 5-30] = xr;
    xr = SQRT2 * (a[12-30] - a[20-30]); a[12-30] += a[20-30]; a[20-30] = xr;
    xr = SQRT2 * (a[17-30] + a[21-30]); a[17-30] -= a[21-30]; a[21-30] = xr;

    xr =-SQRT2 * (a[ 8-30] - a[16-30]); a[ 8-30] += a[16-30]; a[16-30] = xr - a[ 8-30];
    xr =-SQRT2 * (a[ 9-30] - a[13-30]); a[ 9-30] += a[13-30]; a[13-30] = xr - a[ 9-30];
    xr =-SQRT2 * (a[25-30] - a[29-30]); a[25-30] += a[29-30]; a[29-30] = xr - a[25-30];

    xr =-SQRT2 * (a[ 4-30] + a[28-30]);
    a[ 4-30] -= a[28-30];
    a[28-30] = xr - a[ 4-30];

    xr = a[ 4-30] - a[12-30];
    a[ 4-30] = xr;
    xr = a[20-30] - xr;
    a[20-30] = xr;
    xr = a[28-30] - xr;
    a[28-30] = xr;

    a[25-30] -= a[17-30];
    a[21-30] -= a[25-30];
    a[29-30] -= a[21-30];

    a[17-30] -= a[ 1-30];
    a[ 9-30] -= a[17-30];
    a[25-30] -= a[ 9-30];
    a[ 5-30] -= a[25-30];
    a[21-30] -= a[ 5-30];
    a[13-30] -= a[21-30];
    a[29-30] -= a[13-30];

    a[ 1-30] -= a[ 0-30];
    a[12-30] -= a[ 1-30];
    a[17-30] -= a[12-30];
    a[ 8-30] -= a[17-30];
    a[ 9-30] -= a[ 8-30];
    a[ 4-30] -= a[ 9-30];
    a[25-30] -= a[ 4-30];
    a[24-30] -= a[25-30];
    a[ 5-30] -= a[24-30];
    a[20-30] -= a[ 5-30];
    a[21-30] -= a[20-30];
    a[16-30] -= a[21-30];
    a[13-30] -= a[16-30];
    a[28-30] -= a[13-30];
    a[29-30] -= a[28-30];

    xr = a[ 0-30]; a[ 0-30] += a[31-30]; a[31-30] -= xr;
    xr = a[ 1-30]; a[ 1-30] += a[30-30]; a[30-30] -= xr;
    xr = a[12-30]; a[12-30] += a[14-30]; a[14-30] -= xr;
    xr = a[17-30]; a[17-30] += a[15-30]; a[15-30] -= xr;
    xr = a[ 8-30]; a[ 8-30] += a[23-30]; a[23-30] -= xr;
    xr = a[ 9-30]; a[ 9-30] += a[ 6-30]; a[ 6-30] -= xr;
    xr = a[ 4-30]; a[ 4-30] += a[ 7-30]; a[ 7-30] -= xr;
    xr = a[25-30]; a[25-30] += a[22-30]; a[22-30] -= xr;
    xr = a[24-30]; a[24-30] += a[27-30]; a[27-30] -= xr;
    xr = a[ 5-30]; a[ 5-30] += a[ 2-30]; a[ 2-30] -= xr;
    xr = a[20-30]; a[20-30] += a[11-30]; a[11-30] -= xr;
    xr = a[21-30]; a[21-30] += a[10-30]; a[10-30] -= xr;
    xr = a[16-30]; a[16-30] += a[19-30]; a[19-30] -= xr;
    xr = a[13-30]; a[13-30] += a[18-30]; a[18-30] -= xr;
    xr = a[28-30]; a[28-30] += a[ 3-30]; a[ 3-30] -= xr;
    xr = a[29-30]; a[29-30] += a[26-30]; a[26-30] -= xr;
}

}


/*-------------------------------------------------------------------*/
/*                                                                   */
/*   Function: Calculation of the MDCT                               */
/*   In the case of long blocks (type 0,1,3) there are               */
/*   36 coefficents in the time domain and 18 in the frequency       */
/*   domain.                                                         */
/*   In the case of short blocks (type 2) there are 3                */
/*   transformations with short length. This leads to 12 coefficents */
/*   in the time and 6 in the frequency domain. In this case the     */
/*   results are stored side by side in the vector out[].            */
/*                                                                   */
/*   New layer3                                                      */
/*                                                                   */
/*-------------------------------------------------------------------*/

INLINE static void mdct_short(FLOAT8 *inout)
{
    int l;
    for ( l = 0; l < 3; l++ ) {
	FLOAT8 tc0,tc1,tc2,ts0,ts1,ts2;

	ts0 = (inout[2*3] * win[SHORT_TYPE][0] - inout[5*3]) * 1.907525191737280e-11; /* tritab_s[0] */
	tc0 = (inout[0*3] * win[SHORT_TYPE][2] - inout[3*3]) * 1.907525191737280e-11; /* tritab_s[2] */
	tc1 = ts0 + tc0;
	tc2 = ts0 - tc0;

	ts0 = (inout[5*3] * win[SHORT_TYPE][0] + inout[2*3]) * 1.907525191737280e-11; /* tritab_s[2] */
	tc0 = (inout[3*3] * win[SHORT_TYPE][2] + inout[0*3]) * 1.907525191737280e-11; /* tritab_s[0] */
	ts1 = ts0 + tc0;
	ts2 =-ts0 + tc0;

	tc0 = (inout[1*3] * win[SHORT_TYPE][1] - inout[4*3]) * 2.069978111953089e-11; /* tritab_s[1] */
	ts0 = (inout[4*3] * win[SHORT_TYPE][1] + inout[1*3]) * 2.069978111953089e-11; /* tritab_s[1] */

	inout[3*0] = tc1 + tc0;
	inout[3*5] =-ts1 + ts0;

	tc2 = tc2 * cx[6];
	ts1 = ts1 * cx[7] + ts0;
	inout[3*1] = tc2-ts1;
	inout[3*2] = tc2+ts1;

	tc1 = tc1 * cx[7] - tc0;
	ts2 = ts2 * cx[6];
	inout[3*3] = tc1+ts2;
	inout[3*4] = tc1-ts2;

	inout++;
    }
}

INLINE static void mdct_long(FLOAT8 *out, FLOAT8 *in)
{
#define inc(x) in[17-(x)]
#define ins(x) in[8-(x)]

    FLOAT8 tc1 = inc(0)-inc(8),tc2 = (inc(1)-inc(7))*cx[6], tc3 = inc(2)-inc(6), tc4 = inc(3)-inc(5);
    FLOAT8 tc5 = inc(0)+inc(8),tc6 = (inc(1)+inc(7))*cx[7], tc7 = inc(2)+inc(6), tc8 = inc(3)+inc(5);
    FLOAT8 ts1 = ins(0)-ins(8),ts2 = (ins(1)-ins(7))*cx[6], ts3 = ins(2)-ins(6), ts4 = ins(3)-ins(5);
    FLOAT8 ts5 = ins(0)+ins(8),ts6 = (ins(1)+ins(7))*cx[7], ts7 = ins(2)+ins(6), ts8 = ins(3)+ins(5);
    FLOAT8 ct,st;

    ct = tc5+tc7+tc8+inc(1)+inc(4)+inc(7);
    out[0] = ct;

    ct = tc1*cx[0] + tc2 + tc3*cx[1] + tc4*cx[2];
    st = -ts5*cx[4] + ts6 - ts7*cx[5] + ts8*cx[3] + ins(4);
    out[1] = ct+st;
    out[2] = ct-st;

    ct = tc5*cx[3] + tc6 + tc7*cx[4] + tc8*cx[5] - inc(4);
    st = ts1*cx[2] + ts2 + ts3*cx[0] + ts4*cx[1];
    out[3] = ct+st;
    out[4] = ct-st;

    ct = (tc1-tc3-tc4)*cx[6];
    st = (ts5+ts7-ts8)*cx[7]+ins(1)-ins(4)+ins(7);
    out[5] = ct+st;
    out[6] = ct-st;

    ct = -tc5*cx[5] - tc6 - tc7*cx[3] - tc8*cx[4] + inc(4);
    st = ts1*cx[1] + ts2 - ts3*cx[2] - ts4*cx[0];
    out[7] = ct+st;
    out[8] = ct-st;

    ct = tc1*cx[1] - tc2 - tc3*cx[2] + tc4*cx[0];
    st = -ts5*cx[5] + ts6 - ts7*cx[3] + ts8*cx[4] + ins(4);
    out[ 9] = ct+st;
    out[10] = ct-st;

    ct = (tc5+tc7+tc8)*cx[7]-inc(1)-inc(4)-inc(7);
    st = (ts1-ts3+ts4)*cx[6];
    out[11] = ct+st;
    out[12] = ct-st;

    ct = tc1*cx[2] - tc2 + tc3*cx[0] - tc4*cx[1];
    st =  ts5*cx[3] - ts6 + ts7*cx[4] - ts8*cx[5] - ins(4);
    out[13] = ct+st;
    out[14] = ct-st;

    ct = -tc5*cx[4] - tc6 - tc7*cx[5] - tc8*cx[3] + inc(4);
    st = ts1*cx[0] - ts2 + ts3*cx[1] - ts4*cx[2];
    out[15] = ct+st;
    out[16] = ct-st;

    st = ts5+ts7-ts8-ins(1)+ins(4)-ins(7);
    out[17] = st;
#undef inc
#undef ins
}



void mdct_sub48(lame_internal_flags *gfc,
		sample_t *w0, sample_t *w1,
		FLOAT8 mdct_freq[2][2][576])
{
    int gr, k, ch;
    sample_t *wk;

    wk = w0 + 286;
    /* thinking cache performance, ch->gr loop is better than gr->ch loop */
    for (ch = 0; ch < gfc->stereo; ch++) {
	for (gr = 0; gr < gfc->mode_gr; gr++) {
	    int	band;
	    FLOAT8 *mdct_enc = &mdct_freq[gr][ch][0];
	    gr_info *gi = &(gfc->l3_side.gr[gr].ch[ch].tt);
	    FLOAT8 *samp = gfc->sb_sample[ch][1 - gr][0];

	    for (k = 0; k < 18 / 2; k++) {
		window_subband(wk, samp);
		window_subband(wk + 32, samp + 32);
		samp += 64;
		wk += 64;
		/*
		 * Compensate for inversion in the analysis filter
		 */
		for (band = 1; band < 32; band+=2) {
		    samp[band-32] *= -1;
		}
	    }


	    /* apply filters on the polyphase filterbank outputs */
	    /* bands <= gfc->highpass_band will be zeroed out below */
	    /* bands >= gfc->lowpass_band  will be zeroed out below */
	    if (gfc->filter_type==0) {
              for (band=gfc->highpass_start_band;  band <= gfc->highpass_end_band; band++) { 
		  for (k=0; k<18; k++) 
		    gfc->sb_sample[ch][1-gr][k][order[band]]*=gfc->amp_highpass[band];
	      }
              for (band=gfc->lowpass_start_band;  band <= gfc->lowpass_end_band; band++) { 
		  for (k=0; k<18; k++) 
		    gfc->sb_sample[ch][1-gr][k][order[band]]*=gfc->amp_lowpass[band];
	      }
	    }
	    


	    /*
	     * Perform imdct of 18 previous subband samples
	     * + 18 current subband samples
	     */
	    for (band = 0; band < 32; band++, mdct_enc += 18) {
		int type = gi->block_type;
		FLOAT8 *band0, *band1;
		band0 = gfc->sb_sample[ch][  gr][0] + order[band];
		band1 = gfc->sb_sample[ch][1-gr][0] + order[band];
#ifdef ALLOW_MIXED
		if (gi->mixed_block_flag && band < 2)
		    type = 0;
#endif
		if (band >= gfc->lowpass_band || band <= gfc->highpass_band) {
		    memset((char *)mdct_enc,0,18*sizeof(FLOAT8));
		} else {
		  if (type == SHORT_TYPE) {
		    for (k = -NS/4; k < 0; k++) {
			FLOAT8 w = win[SHORT_TYPE][k+3];
			mdct_enc[k*3+ 9] = band0[( 9+k)*32] * w - band0[( 8-k)*32];
			mdct_enc[k*3+18] = band0[(14-k)*32] * w + band0[(15+k)*32];
			mdct_enc[k*3+10] = band0[(15+k)*32] * w - band0[(14-k)*32];
			mdct_enc[k*3+19] = band1[( 2-k)*32] * w + band1[( 3+k)*32];
			mdct_enc[k*3+11] = band1[( 3+k)*32] * w - band1[( 2-k)*32];
			mdct_enc[k*3+20] = band1[( 8-k)*32] * w + band1[( 9+k)*32];
		    }
		    mdct_short(mdct_enc);
		  } else {
		    FLOAT8 work[18];
		    for (k = -NL/4; k < 0; k++) {
			FLOAT8 a, b;
			a = win[type][k+27] * band1[(k+9)*32]
			  + win[type][k+36] * band1[(8-k)*32];
			b = win[type][k+ 9] * band0[(k+9)*32]
			  - win[type][k+18] * band0[(8-k)*32];
			work[k+ 9] = a - b*tantab_l[k+9];
			work[k+18] = a*tantab_l[k+9] + b;
		    }

		    mdct_long(mdct_enc, work);
		  }
		}
		/*
		 * Perform aliasing reduction butterfly
		 */
		if (type != SHORT_TYPE) {
		  if (band == 0)
		    continue;
		  for (k = 7; k >= 0; --k) {
		    FLOAT8 bu,bd;
		    bu = mdct_enc[k] * ca[k] + mdct_enc[-1-k] * cs[k];
		    bd = mdct_enc[k] * cs[k] - mdct_enc[-1-k] * ca[k];
		    
		    mdct_enc[-1-k] = bu;
		    mdct_enc[k]    = bd;
		  }
		}
	      }
	}
	wk = w1 + 286;
	if (gfc->mode_gr == 1) {
	    memcpy(gfc->sb_sample[ch][0], gfc->sb_sample[ch][1], 576 * sizeof(FLOAT8));
	}
    }
}
