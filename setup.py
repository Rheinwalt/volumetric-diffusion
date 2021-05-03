from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

setup(name = 'vdiffusion',
      version = '0.1',
      url = 'https://github.com/rheinwalt',
      author = 'Aljoscha Rheinwalt',
      author_email = 'aljoscha.rheinwalt@uni-potsdam.de',
      ext_modules = cythonize([Extension('vdiffusion', ['vdiffusion.pyx'],
                               extra_compile_args = ['-fopenmp'],
                               extra_link_args = ['-fopenmp'],
                               )])
)
