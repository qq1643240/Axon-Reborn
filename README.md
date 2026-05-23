# Axon Reborn - iOS 14-16 无根/隐根越狱版

## 主要改进

### 1. 支持 iOS 14-16 无根/隐根越狱
- 使用 `libroot` 库处理路径映射 (`jbroot()`)
- Makefile 配置 `THEOS_PACKAGE_SCHEME = rootless`
- 架构支持 `arm64` + `arm64e`
- 依赖 `com.opa334.libroot`

### 2. 新增手势清除通知功能
- **上滑图标**：清除该应用的**所有通知**
- **下滑图标**：清除**全部应用**的所有通知
- 可在设置面板中独立开关这两个手势
- 手势触发时有触觉反馈和滑出动画

### 3. 保留原有功能
- 长按菜单清除（单个/全部）
- Priority Hub 通知分组
- 多种样式和布局选项
- 暗色模式支持

## 编译要求

1. 安装 [Theos](https://theos.dev/)
2. 安装 `libroot` 开发库
3. iOS 14.0+ SDK

## 编译步骤

```bash
cd Axon-Reborn
make clean
make package
```

## 安装

编译完成后会生成 `.deb` 文件，通过 Filza 或 Sileo 安装即可。

## 文件结构

```
Axon-Reborn/
├── Makefile                          # 主 Makefile (rootless 配置)
├── control                           # 包信息
├── layout/DEBIAN/control             # 安装包控制文件
├── Tweak/
│   ├── Tweak.xm                      # 主注入文件 (支持 iOS 14-16)
│   ├── Tweak.h                       # 头文件
│   ├── AXNView.h/.m                  # 通知视图 (添加手势开关)
│   ├── AXNManager.h/.m               # 通知管理器 (libroot 路径)
│   ├── AXNAppCell.h/.m               # 应用图标单元格 (上滑/下滑手势)
│   ├── AXNRequestWrapper.h/.m        # 请求包装器
│   ├── RandomHeaders.h               # 私有头文件
│   └── Protocol.h                    # 协议定义
└── Preferences/
    ├── Makefile                      # 设置面板 Makefile
    ├── entry.plist                   # PreferenceLoader 入口
    ├── AXNRootListController.h/.m    # 设置控制器
    └── Resources/Root.plist          # 设置项 (含手势开关)
```

## 技术细节

### 无根越狱适配
- 所有硬编码路径 `/var/mobile/...` 改为 `jbroot(@"/var/mobile/...")`
- 使用 `libroot` 提供的 `jbroot()` 宏自动处理路径映射

### 手势实现
在 `AXNAppCell.m` 中添加了 `UISwipeGestureRecognizer`：
```objc
// 上滑 - 清除单个应用
UISwipeGestureRecognizer *swipeUp = [[UISwipeGestureRecognizer alloc] 
    initWithTarget:self action:@selector(handleSwipeUp:)];
swipeUp.direction = UISwipeGestureRecognizerDirectionUp;

// 下滑 - 清除全部
UISwipeGestureRecognizer *swipeDown = [[UISwipeGestureRecognizer alloc] 
    initWithTarget:self action:@selector(handleSwipeDown:)];
swipeDown.direction = UISwipeGestureRecognizerDirectionDown;
```

手势触发时：
1. 播放触觉反馈 (`AudioServicesPlaySystemSound`)
2. 执行滑出动画 (`UIView animateWithDuration`)
3. 调用 `AXNManager` 的清除方法
4. 刷新视图

## 兼容性

- **iOS 版本**: 14.0 - 16.x
- **越狱类型**: Dopamine, Palera1n, XinaA15 等无根/隐根越狱
- **架构**: arm64, arm64e

## 许可证

基于原 Axon 项目 (GPL v3) 修改。

原作者: Nepeta & Baw Appie
修改: Axon Reborn Team


## 🚀 GitHub Actions 自动编译

本项目已配置 GitHub Actions，推送代码后自动编译 `.deb` 文件。

### 使用方法

1. **Fork 本项目** 到你的 GitHub 账号
2. **修改代码**（可选）
3. **推送代码** 到 main 分支
4. **查看 Actions** 标签页，等待编译完成
5. **下载 Artifacts** 中的 `.deb` 文件

### 发布 Release

推送 tag 时自动发布 Release：
```bash
git tag v2.0
git push origin v2.0
```

Release 页面会自动上传 `.deb` 文件。

### 手动触发编译

进入 Actions 标签页 → 选择 "Build Axon Reborn" → 点击 "Run workflow"
