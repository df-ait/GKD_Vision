#!/bin/bash

blue="\033[1;34m"
yellow="\033[1;33m"
red="\033[1;31m"
reset="\033[0m"

# 计算代码行数
include_count=$(find include -type f \( -name "*.h"  -o -name "*.cuh" \) -exec cat {} \; | wc -l)
src_count=$(find src -type f \( -name "*.cpp" -o -name "*.cu" -o -name "*.txt" \) -exec cat {} \; | wc -l)
total=$((include_count + src_count))

max_threads=$(nproc)

if [ ! -d "build" ]; then 
    mkdir build
fi

while getopts ":ritdg:" opt; do
    case $opt in
        r)
            echo -e "${yellow}<<<--- rebuild --->>>\n${reset}"
            cd build
            sudo make uninstall
            cd ..
            sudo rm -rf build
            mkdir build
            ;;
        i)
            echo -e "${yellow}<<<--- reinstall --->>>\n${reset}"
            cd build
            sudo make uninstall
            cd ..
            ;;
        t)
            echo -e "${yellow}<<<--- terminal --->>>\n${reset}"
            cd build
            cmake ..
            sudo make install
            cd ../terminal
            if [ ! -d "build" ]; then 
                mkdir build
            fi
            cd build
            cmake ..
            make -j "$max_threads"
            make -j "$max_threads"
            sudo make install
            cd ../..
            exit 0
            ;;
        d)
            echo -e "${yellow}\nThe following files and directories will be deleted:${reset}"
            sudo find "$(pwd)" -maxdepth 1 -name "build"
            sudo find /usr/local/ -name "*GKD_Vision*"
            
            echo -e "${red}Are you sure you want to delete GKD_Vision? (y/n): ${reset}"
            read answer
            if [ "$answer" == "y" ] || [ "$answer" == "Y" ]; then
                echo -e "${yellow}\n<<<--- delete --->>>\n${reset}"
                sudo find "$(pwd)" -maxdepth 1 -name "build" -exec rm -rf {} +
                sudo find /usr/local/ -name "*GKD_Vision*" -exec rm -rf {} +
            else
                echo -e "${yellow}Deletion operation canceled${reset}"
            fi
            exit 0
            ;;
        g)
            git_message=$OPTARG
            echo -e "${yellow}\n<--- Git $git_message --->${reset}"
            git pull
            git add -A
            git commit -m "$git_message"
            git push
            exit 0
            ;;
        \?)
            echo -e "${red}\n--- Unavailable param: -$OPTARG ---\n${reset}"
            ;;
        :)
            echo -e "${red}\n--- param -$OPTARG need a value ---\n${reset}"
            ;;
    esac
done

echo -e "${yellow}\n<<<--- start cmake --->>>\n${reset}"
cd build
cmake ..

echo -e "${yellow}\n<<<--- start make --->>>\n${reset}"
make -j "$max_threads"

echo -e "${yellow}\n<<<--- start install --->>>\n${reset}"
sudo make install
sudo rm /usr/lib/libGKD_Vision_*
sudo ln -s /usr/local/lib/libGKD_Vision_* /usr/lib

echo -e "${yellow}\n<<<--- Total Lines --->>>${reset}"
echo -e "${blue}           $total${reset}"

echo -e "${yellow}\n<<<--- Welcome GKD Vision --->>>\n${reset}"