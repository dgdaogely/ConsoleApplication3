// ConsoleApplication3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#ifndef __AnimateActors_h
#include <vector>
#include <vtkActor.h>
#include <vtkAnimationCue.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>

class ActorAnimator
{
public:
    ActorAnimator()
    {
        this->Actor = 0;
        this->Observer = AnimationCueObserver::New();
        this->Observer->Animator = this;
        this->StartPosition.resize(3);
        this->StartPosition.insert(this->StartPosition.begin(), 3, 0.0);
        this->EndPosition.resize(3);
        this->EndPosition.insert(this->EndPosition.begin(), 3, .5);
    }

    ~ActorAnimator()
    {
        if (this->Actor)
        {
            this->Actor->UnRegister(0);
            this->Actor = 0;
        }
        this->Observer->UnRegister(0);
    }
    void SetActor(vtkActor* actor)
    {
        if (this->Actor)
        {
            this->Actor->UnRegister(0);
        }
        this->Actor = actor;
        this->Actor->Register(0);
    }
    void SetStartPosition(std::vector<double> const& position)
    {
        this->StartPosition = position;
    }
    void SetEndPosition(std::vector<double> const& position)
    {
        this->EndPosition = position;
    }
    void AddObserversToCue(vtkAnimationCue* cue)
    {
        cue->AddObserver(vtkCommand::StartAnimationCueEvent, this->Observer);
        cue->AddObserver(vtkCommand::EndAnimationCueEvent, this->Observer);
        cue->AddObserver(vtkCommand::AnimationCueTickEvent, this->Observer);
    }

    void Start(vtkAnimationCue::AnimationCueInfo* vtkNotUsed(info))
    {
        this->Actor->SetPosition(this->StartPosition[0], this->StartPosition[1],
            this->StartPosition[2]);
    }

    void Tick(vtkAnimationCue::AnimationCueInfo* info)
    {
        double t = (info->AnimationTime - info->StartTime) /
            (info->EndTime - info->StartTime);
        double position[3];
        for (int i = 0; i < 3; i++)
        {
            position[i] = this->StartPosition[i] +
                (this->EndPosition[i] - this->StartPosition[i]) * t;
        }
        this->Actor->SetPosition(position);
    }

    void End(vtkAnimationCue::AnimationCueInfo* vtkNotUsed(info))
    {
        this->Actor->SetPosition(this->EndPosition[0], this->EndPosition[1],
            this->EndPosition[2]);
    }

protected:
    class AnimationCueObserver : public vtkCommand
    {
    public:
        static AnimationCueObserver* New()
        {
            return new AnimationCueObserver;
        }

        virtual void Execute(vtkObject* vtkNotUsed(caller), unsigned long event,
            void* calldata)
        {
            if (this->Animator != 0)
            {
                vtkAnimationCue::AnimationCueInfo* info =
                    static_cast<vtkAnimationCue::AnimationCueInfo*>(calldata);
                switch (event)
                {
                case vtkCommand::StartAnimationCueEvent:
                    this->Animator->Start(info);
                    break;
                case vtkCommand::EndAnimationCueEvent:
                    this->Animator->End(info);
                    break;
                case vtkCommand::AnimationCueTickEvent:
                    this->Animator->Tick(info);
                    break;
                }
            }
        }

        AnimationCueObserver()
        {
            this->Animator = 0;
        }
        ActorAnimator* Animator;
    };

    vtkActor* Actor;
    AnimationCueObserver* Observer;
    std::vector<double> StartPosition;
    std::vector<double> EndPosition;
};

class AnimationSceneObserver : public vtkCommand
{
public:
    static AnimationSceneObserver* New()
    {
        return new AnimationSceneObserver;
    }

    void SetRenderWindow(vtkRenderWindow* renWin)
    {
        if (this->RenderWindow)
        {
            this->RenderWindow->UnRegister(this);
        }
        this->RenderWindow = renWin;
        this->RenderWindow->Register(this);
    }
    virtual void Execute(vtkObject* vtkNotUsed(caller), unsigned long event,
        void* vtkNotUsed(calldata))
    {
        if (this->RenderWindow != 0)
        {
            switch (event)
            {
            case vtkCommand::AnimationCueTickEvent:
                this->RenderWindow->Render();
                break;
            }
        }
    }

protected:
    AnimationSceneObserver()
    {
        this->RenderWindow = 0;
    }
    ~AnimationSceneObserver()
    {
        if (this->RenderWindow)
        {
            this->RenderWindow->UnRegister(this);
            this->RenderWindow = 0;
        }
    }
    vtkRenderWindow* RenderWindow;
};

#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
