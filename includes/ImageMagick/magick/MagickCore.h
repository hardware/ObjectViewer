/*
  Copyright 1999-2013 ImageMagick Studio LLC, a non-profit organization
  dedicated to making software imaging solutions freely available.

  You may not use this file except in compliance with the License.
  obtain a copy of the License at

    http://www.imagemagick.org/script/license.php

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  MagickCore Application Programming Interface declarations.
*/

#ifndef _MAGICKCORE_CORE_H
#define _MAGICKCORE_CORE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(_MAGICKCORE_CONFIG_H)
# define _MAGICKCORE_CONFIG_H
# if !defined(vms) && !defined(macintosh)
#  include "magick-config.h"
# else
#  include "magick-config.h"
# endif
#if defined(_magickcore_const) && !defined(const)
# define const _magickcore_const
#endif
#if defined(_magickcore_inline) && !defined(inline)
# define inline _magickcore_inline
#endif
#if defined(_magickcore_restrict) && !defined(restrict)
# define restrict  _magickcore_restrict
#endif
# if defined(__cplusplus) || defined(c_plusplus)
#  undef inline
# endif
#endif

#define MAGICKCORE_CHECK_VERSION(major,minor,micro) \
  ((MAGICKCORE_MAJOR_VERSION > (major)) || \
    ((MAGICKCORE_MAJOR_VERSION == (major)) && \
     (MAGICKCORE_MINOR_VERSION > (minor))) || \
    ((MAGICKCORE_MAJOR_VERSION == (major)) && \
     (MAGICKCORE_MINOR_VERSION == (minor)) && \
     (MAGICKCORE_MICRO_VERSION >= (micro))))

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(WIN32) || defined(WIN64)
#  define MAGICKCORE_WINDOWS_SUPPORT
#else
#  define MAGICKCORE_POSIX_SUPPORT
#endif

#include "method-attribute.h"

#if defined(MAGICKCORE_NAMESPACE_PREFIX)
# include "magick/methods.h"
#endif
#include "magick-type.h"
#include "accelerate.h"
#include "animate.h"
#include "annotate.h"
#include "artifact.h"
#include "attribute.h"
#include "blob.h"
#include "cache.h"
#include "cache-view.h"
#include "channel.h"
#include "cipher.h"
#include "client.h"
#include "coder.h"
#include "color.h"
#include "colorspace.h"
#include "colormap.h"
#include "compare.h"
#include "composite.h"
#include "compress.h"
#include "configure.h"
#include "constitute.h"
#include "decorate.h"
#include "delegate.h"
#include "deprecate.h"
#include "display.h"
#include "distort.h"
#include "distribute-cache.h"
#include "draw.h"
#include "effect.h"
#include "enhance.h"
#include "exception.h"
#include "feature.h"
#include "fourier.h"
#include "fx.h"
#include "gem.h"
#include "geometry.h"
#include "hashmap.h"
#include "histogram.h"
#include "identify.h"
#include "image.h"
#include "image-view.h"
#include "layer.h"
#include "list.h"
#include "locale_.h"
#include "log.h"
#include "magic.h"
#include "magick.h"
#include "matrix.h"
#include "memory_.h"
#include "module.h"
#include "mime.h"
#include "monitor.h"
#include "montage.h"
#include "morphology.h"
#include "option.h"
#include "paint.h"
#include "pixel.h"
#include "pixel-accessor.h"
#include "policy.h"
#include "prepress.h"
#include "profile.h"
#include "property.h"
#include "quantize.h"
#include "quantum.h"
#include "registry.h"
#include "random_.h"
#include "resample.h"
#include "resize.h"
#include "resource_.h"
#include "segment.h"
#include "shear.h"
#include "signature.h"
#include "splay-tree.h"
#include "stream.h"
#include "statistic.h"
#include "string_.h"
#include "timer.h"
#include "token.h"
#include "transform.h"
#include "threshold.h"
#include "type.h"
#include "utility.h"
#include "version.h"
#include "xml-tree.h"
#include "xwindow.h"

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
