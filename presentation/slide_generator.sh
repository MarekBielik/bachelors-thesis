#!/usr/bin/env bash

rm -f evolution-course.tex

for i in {00..50}; do
    frame="\begin{frame}{Evolution example}
\centering
\includegraphics[scale=0.45]{evolutionCourse/graph${i}}
\end{frame}";

    echo ${frame} >> evolution-course.tex;
done

echo "Slides generated.";