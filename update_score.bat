@echo off

REM Check if score.txt has been modified
git diff --quiet HEAD score.txt
if %errorlevel% == 0 (
    echo No changes in score.txt, skipping update.
    exit /b 0
)

REM If we're here, score.txt has been modified
echo Pushing score to GitHub...
git add score.txt
git commit -m "[Auto]Update score"
git push origin master