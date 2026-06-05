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
    Created:  16/05/26  @  6:50 PM
    FileName: ObjectMarker.hpp @ RedSoUL Project
    History:
             - created by: 16/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t
/// Library headers
#include "Common/CommonDefines.hpp"   /// INLINE_FUNCTION
#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "Text/StaticStringId.hpp"
#include "Text/StaticString.hpp"


struct CreateParameter;
class  ObjectMarker;
class  SceneObject;


/// 物体属性的创建函数
/// @return
///     新创建的ObjectMarker实例, 如果成功
///     nullptr, 如果失败
typedef ObjectMarker * (*CreateFuncPtr)(const CreateParameter &);
/// 物体属性的销毁函数
typedef bool (*DestroyFuncPtr)(ObjectMarker *&);


/// 物体属性创建参数
struct CreateParameter
{
    /// 属性的所有者
    SceneObject &   owner;
    /// 属性类型名称Id
    StaticStringIdT name_id;

    CreateParameter (
        const StaticStringIdT name_id,
        SceneObject &         owner);
};


/// 属性类型信息
struct MarkerTypeInfo
{
    const CreateFuncPtr   create_func;
    const DestroyFuncPtr  destroy_func;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    const StaticString    marker_name;

    MarkerTypeInfo (
        const char * const   marker_name,
        const CreateFuncPtr  create_func,
        const DestroyFuncPtr destroy_func);

    StaticStringIdT
    name_id () const;

#else
    const StaticStringIdT marker_name_id;

    MarkerTypeInfo (
        const StaticStringIdT name_id,
        const CreateFuncPtr   create_func,
        const DestroyFuncPtr  destroy_func);

    StaticStringIdT
    name_id () const;
#endif
};


/// 物体属性基类
///
class ObjectMarker
{
protected:
    ObjectMarker (
        const StaticStringIdT name_id,
        SceneObject &         owner);

    ~ObjectMarker ();

    ObjectMarker (
        const ObjectMarker &) = delete;
    ObjectMarker & operator = (
        const ObjectMarker &) = delete;

protected:
    friend class SceneObject;

    SceneObject &   m_owner;
    StaticStringIdT m_name_id;
};
