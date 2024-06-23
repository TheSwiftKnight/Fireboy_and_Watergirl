cmake_minimum_required(VERSION 3.27)
project(2024_I2P2_TowerDefense_with_answer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2024_I2P2_TowerDefense_with_answer
        Engine/Allegro5Exception.cpp
        Engine/Allegro5Exception.hpp
        Engine/AudioHelper.cpp
        Engine/AudioHelper.hpp
        Engine/Collider.cpp
        Engine/Collider.hpp
        Engine/GameEngine.cpp
        Engine/GameEngine.hpp
        Engine/Group.cpp
        Engine/Group.hpp
        Engine/IControl.cpp
        Engine/IControl.hpp
        UI/Component/Image.cpp
        UI/Component/Image.hpp
        UI/Component/ImageButton.cpp
        UI/Component/ImageButton.hpp
        Engine/IObject.cpp
        Engine/IObject.hpp
        Engine/IScene.cpp
        Engine/IScene.hpp
        UI/Component/Label.cpp
        UI/Component/Label.hpp
        machine/Button.cpp
        machine/Button.hpp
        machine/Diamond.cpp
        machine/Diamond.hpp
        machine/Elevator.cpp
        machine/Elevator.hpp
        machine/Lever.cpp
        machine/Lever.hpp
        machine/Stone.cpp
        machine/Stone.hpp
        machine/Switch.cpp
        machine/Switch.hpp
        Engine/LOG.cpp
        Engine/LOG.hpp
        Scene/LoseScene.cpp
        Scene/LoseScene.hpp
        main.cpp
        Scene/PlayScene.cpp
        Scene/PlayScene.hpp
        Engine/Point.cpp
        Engine/Point.hpp
        Engine/Resources.cpp
        Engine/Resources.hpp
        UI/Component/Slider.cpp
        UI/Component/Slider.hpp
        Engine/Sprite.cpp
        Engine/Sprite.hpp
        Scene/StageSelectScene.cpp
        Scene/StageSelectScene.hpp
        Scene/WinScene.cpp
        Scene/WinScene.hpp
        # TODO: [HACKATHON-1-SCENE] (2/4) Add the new scene source files here
        Scene/StartScene.cpp
        Scene/StartScene.hpp
        Scene/SettingsScene.cpp
        Scene/SettingsScene.hpp
        Scene/ScoreBoardScene.cpp
        Scene/ScoreBoardScene.hpp
        Twins/Twins.cpp
        Twins/Twins.hpp
#        UI/Component/TextInput.cpp
#        UI/Component/TextInput.hpp
)

if(WIN32)
    set(UPDATE_SCRIPT ${CMAKE_SOURCE_DIR}/update_score.bat)
else()
    set(UPDATE_SCRIPT ${CMAKE_SOURCE_DIR}/update_score.sh)
endif()

# Create a custom target for updating the score
add_custom_target(update_score ALL
    COMMAND ${UPDATE_SCRIPT}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_dependencies(2024_I2P2_TowerDefense_with_answer update_score)


IF (WIN32)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC C:/allegro/include)
    target_link_directories(2024_I2P2_TowerDefense_with_answer PUBLIC C:/allegro/lib)
    target_compile_options(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2024_I2P2_TowerDefense_with_answer liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC lib)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2024_I2P2_TowerDefense_with_answer ${ALLEGRO5_LIBRARIES})
    target_link_directories(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()