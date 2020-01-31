#!/usr/bin/env python
from setuptools import setup

setup(name='btk',
      version='0.4',
      description='BTKCore',
      author='Arnaud Barré',
      author_email='',
      url='https://github.com/Biomechanical-ToolKit/BTKCore',
      license='BSD license',
      packages=['btk'],
      package_data={'btk': ['__init__.py',
                            'btk.py',
                            '*.so*',
                            '*.dll',
                            '*.dylib']},
      include_package_data=True)
