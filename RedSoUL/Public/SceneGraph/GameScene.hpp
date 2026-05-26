/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  13/05/26  @  10:05 PM
    FileName: GameScene.hpp @ RedSoUL Project
    History:
             - created by: 13/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <vector>


class SceneObject;


/// 游戏场景
class GameScene
{
public:
    /// 获取活跃的场景实例
    /// NOTE: 只有一个场景活跃
    ///
    /// @return
    ///     GameScene的实例, 如果有活跃场景
    ///     nullptr,        如果无活跃场景
    static
    GameScene *
    active_scene ();

    /// 创建一个空场景实例
    ///
    /// @return
    ///     空场景的指针, 如果创建成功
    ///     nullptr,     如果创建失败(NOTE: 此时原有的活跃场景实例依旧有效)
    static
    GameScene *
    create_empty_scene ();

    /// 创建SceneObject
    ///
    /// @param[in]  father
    ///     父节点(nullptr, 如果无)
    SceneObject *
    create_object (
        SceneObject * const father);

    /// 销毁SceneObject
    void
    destroy_object (
        SceneObject *& scene_object);


private:
    GameScene ();

    ~GameScene ();

    GameScene (
        const GameScene &) = delete;
    GameScene & operator = (
        const GameScene &) = delete;

    /// 销毁所有SceneObject
    void
    destroy_all_objects ();

private:
    typedef std::vector<SceneObject*> SceneObjectListT;

    // === SCENE GRAPH === //
    /// 所有SceneObject的列表
    SceneObjectListT m_object_list;
    /// TOPELEVEL(所有场景ROOT根节点的子节点列表)SceneObject列表
    SceneObjectListT m_toplevel_object_list;
};
