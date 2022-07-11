# ====================================================================================================================================
# @file       conf.py
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Monday, 23rd May 2022 11:52:34 pm
# @modified   Monday, 11th July 2022 2:07:23 pm
# @project    ethercat-lib
# @brief      Configuration file for the Sphinx documentation builder.
# 
# 
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ======================================================= Project information ====================================================== #

# Project info
project = 'ethercat'
copyright = 'Krzysztof Pierczyk © 2022'
author = 'Krzysztof Pierczyk'

# The short X.Y version
version = '1.0'
# The full version, including alpha/beta/rc tags
release = '1.0.0'

# ====================================================== General configuration ===================================================== #

# Minimal version of Sphinx
needs_sphinx = '4.5'
# Doc's language
language = 'en'

# Sphinx extensions
extensions = [ 'breathe' ]

# Templates sources
templates_path = [ ]
# Source suffixes
source_suffix = [ '.rst' ]
# The master toctree document.
master_doc = 'index'

# ----------------------------------------------------------------------------------
# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# ----------------------------------------------------------------------------------
exclude_patterns = [ ]

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = None

# ===================================================== Options for HTML output ==================================================== #

# The theme to use for HTML and HTML Help pages
html_theme = 'sphinx_rtd_theme'

# ----------------------------------------------------------------------------------
# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
# ----------------------------------------------------------------------------------
# html_theme_options = { }

# ----------------------------------------------------------------------------------
# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
# ----------------------------------------------------------------------------------
html_static_path = [ ]

# ----------------------------------------------------------------------------------
# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
# ----------------------------------------------------------------------------------
# html_sidebars = { }

# =================================================== Options for HTMLHelp output ================================================== #

# ----------------------------------------------------------------------------------
# Output file base name for HTML help builder.
# ----------------------------------------------------------------------------------
htmlhelp_basename = 'projectdoc'

# ==================================================== Options for LaTeX output ==================================================== #

# ----------------------------------------------------------------------------------
# Latex output elements
# ----------------------------------------------------------------------------------
latex_elements = {

    # The paper size ('letterpaper' or 'a4paper').
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    # 'preamble': '',

    # Latex figure (float) alignment
    # 'figure_align': 'htbp',
    
}

# ----------------------------------------------------------------------------------
# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,  author, documentclass [howto, manual, 
# or own class]).
# ----------------------------------------------------------------------------------
latex_documents = [ (master_doc, 'project.tex', project, author, 'manual') ]

# ================================================================================================================================== #
