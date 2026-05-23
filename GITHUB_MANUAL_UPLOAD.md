# Axon Reborn - GitHub 网页手动上传指南

## 适用场景

- 不想安装 Git
- 不想用命令行
- 直接在 GitHub 网页操作

---

## 第一步：创建 GitHub 仓库

1. 打开 [github.com](https://github.com)，登录你的账号
2. 点击右上角 **+** → **New repository**
3. 填写信息：
   - **Repository name**: `Axon-Reborn`
   - **Description**: `Priority Hub for iOS 14-16 (Rootless/RootHide)`
   - 选择 **Public**（免费）
   - 勾选 **Add a README file**
4. 点击 **Create repository**

---

## 第二步：上传项目文件

### 方法 A：直接拖拽上传（推荐，最简单）

1. 进入你刚创建的仓库页面
2. 点击 **Add file** → **Upload files**
3. 打开你电脑上的 `Axon-Reborn` 文件夹
4. **全选所有文件和文件夹**（Ctrl+A）
5. **拖拽到 GitHub 网页的上传区域**
6. 等待上传完成
7. 在 **Commit changes** 区域填写：
   - Commit message: `Initial commit`
8. 点击 **Commit changes**

### 方法 B：逐个文件夹上传

如果拖拽上传有问题，可以分批上传：

**上传根目录文件：**
1. 点击 **Add file** → **Upload files**
2. 选择这些文件上传：
   - `Makefile`
   - `README.md`
   - `.gitignore`
3. Commit message: `Add root files`
4. 点击 **Commit changes**

**创建 Tweak 文件夹并上传：**
1. 点击 **Add file** → **Create new file**
2. 在文件名输入框输入：`Tweak/.gitkeep`
3. 点击 **Commit new file**
   - 这样会自动创建 `Tweak` 文件夹
4. 进入 `Tweak` 文件夹
5. 点击 **Add file** → **Upload files**
6. 选择 `Tweak` 文件夹里的所有文件上传：
   - `Tweak.xm`
   - `Tweak.h`
   - `AXNView.h`
   - `AXNView.m`
   - `AXNManager.h`
   - `AXNManager.m`
   - `AXNAppCell.h`
   - `AXNAppCell.m`
   - `AXNRequestWrapper.h`
   - `AXNRequestWrapper.m`
   - `RandomHeaders.h`
   - `Protocol.h`
7. Commit message: `Add Tweak files`
8. 点击 **Commit changes**

**创建 Preferences 文件夹：**
1. 回到仓库根目录
2. 点击 **Add file** → **Create new file**
3. 文件名：`Preferences/.gitkeep`
4. 点击 **Commit new file**
5. 进入 `Preferences` 文件夹
6. 上传里面的文件：
   - `Makefile`
   - `entry.plist`
   - `AXNRootListController.h`
   - `AXNRootListController.m`
7. Commit message: `Add Preferences files`

**创建 Preferences/Resources 文件夹：**
1. 进入 `Preferences` 文件夹
2. 点击 **Add file** → **Create new file**
3. 文件名：`Preferences/Resources/.gitkeep`
4. 点击 **Commit new file**
5. 进入 `Preferences/Resources`
6. 上传 `Root.plist`
7. Commit message: `Add Preferences Resources`

**创建 layout/DEBIAN 文件夹：**
1. 回到仓库根目录
2. 点击 **Add file** → **Create new file**
3. 文件名：`layout/DEBIAN/.gitkeep`
4. 点击 **Commit new file**
5. 进入 `layout/DEBIAN`
6. 上传 `control` 文件
7. Commit message: `Add DEBIAN control`

**创建 .github/workflows 文件夹：**
1. 回到仓库根目录
2. 点击 **Add file** → **Create new file**
3. 文件名：`.github/workflows/build.yml`
4. 把下面的 workflow 代码复制进去
5. 点击 **Commit new file**

---

## 第三步：添加 GitHub Actions 配置

如果上面的 `.github/workflows/build.yml` 没有上传成功，手动创建：

1. 在仓库根目录点击 **Add file** → **Create new file**
2. 文件名必须是：`.github/workflows/build.yml`
3. 把下面的代码完整复制到编辑框：

```yaml
name: Build Axon Reborn

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]
  workflow_dispatch:

jobs:
  build:
    runs-on: macos-latest

    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Setup Theos
      uses: actions/checkout@v4
      with:
        repository: theos/theos
        path: theos
        submodules: recursive

    - name: Install dependencies
      run: |
        brew install dpkg ldid

    - name: Setup Theos environment
      run: |
        echo "THEOS=$(pwd)/theos" >> $GITHUB_ENV
        echo "THEOS_DEVICE_IP=127.0.0.1" >> $GITHUB_ENV

    - name: Download iOS SDK
      run: |
        mkdir -p $THEOS/sdks
        curl -L -o $THEOS/sdks/iPhoneOS14.5.sdk.tar.gz \
          "https://github.com/chrisharper22/sdks/releases/download/iOS14.5/iPhoneOS14.5.sdk.tar.gz"
        cd $THEOS/sdks && tar -xzf iPhoneOS14.5.sdk.tar.gz && rm iPhoneOS14.5.sdk.tar.gz

    - name: Download libroot
      run: |
        mkdir -p $THEOS/lib
        curl -L -o $THEOS/lib/libroothide.dylib \
          "https://raw.githubusercontent.com/opa334/libroot/main/libroothide.dylib" || true

    - name: Build Tweak
      run: |
        export THEOS=$(pwd)/theos
        make clean
        make package

    - name: Upload DEB artifact
      uses: actions/upload-artifact@v4
      with:
        name: Axon-Reborn-Deb
        path: packages/*.deb

    - name: Create Release
      if: github.event_name == 'push' && contains(github.ref, 'refs/tags/')
      uses: softprops/action-gh-release@v1
      with:
        files: packages/*.deb
        body: |
          Axon Reborn - iOS 14-16 Rootless/RootHide

          ## 新功能
          - 支持 iOS 14-16 无根/隐根越狱
          - 上滑图标清除单个应用通知
          - 下滑图标清除全部通知

          ## 安装
          通过 Sileo/Filza 安装此 deb 包
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

4. 点击 **Commit new file**
5. Commit message: `Add GitHub Actions workflow`

---

## 第四步：触发编译

### 方法 A：修改任意文件触发

1. 在仓库根目录点击任意文件（比如 `README.md`）
2. 点击右上角的 **✏️ Edit** 按钮
3. 随便加一点内容（比如在末尾加个空格）
4. 点击 **Commit changes**
5. 填写 Commit message: `Trigger build`
6. 点击 **Commit changes**

### 方法 B：创建 Release Tag 触发

1. 点击仓库右侧的 **Releases**
2. 点击 **Create a new release**
3. 点击 **Choose a tag** → 输入 `v2.0` → 点击 **Create new tag: v2.0**
4. Release title: `Axon Reborn v2.0`
5. Description:
```
Axon Reborn - iOS 14-16 无根/隐根越狱版

## 新功能
- 支持 iOS 14-16 Rootless/RootHide 越狱
- 上滑图标清除单个应用通知
- 下滑图标清除全部通知
- 可在设置中开关手势

## 安装
通过 Sileo 或 Filza 安装 .deb 文件

## 依赖
- mobilesubstrate
- com.opa334.libroot
```
6. 点击 **Publish release**

---

## 第五步：下载编译好的 .deb

### 从 Actions 下载（推荐）

1. 点击仓库顶部的 **Actions** 标签
2. 看到 `Build Axon Reborn` 正在运行（黄色 🟡）
3. 等待变成绿色 ✅（约 3-5 分钟）
4. 点击那次成功的运行记录
5. 页面下方找到 **Artifacts** 区域
6. 点击 `Axon-Reborn-Deb` 下载
7. 解压下载的 zip 文件，里面就是 `.deb`

### 从 Releases 下载

1. 点击仓库右侧的 **Releases**
2. 找到 `v2.0`
3. 在 Assets 下面下载 `.deb` 文件

---

## 第六步：安装到 iPhone

### 方法 1：Filza（最简单）

1. 把 `.deb` 传到手机：
   - 微信文件传输助手
   - QQ
   - 数据线 + iTunes
   - AirDrop（Mac）
2. 手机上用 **Filza** 打开 `.deb`
3. 点击 **安装**
4. 安装完成后点击 **注销**（Respring）

### 方法 2：Sileo

1. 把 `.deb` 放到 `/var/mobile/Documents/`
2. 打开 Sileo
3. 软件源 → 右上角 `+`
4. 输入：`file:///var/mobile/Documents/`
5. 刷新后找到 Axon Reborn 安装

---

## ⚠️ 常见问题

### 问题 1：Actions 编译失败（红色 ❌）

**查看日志：**
1. 点击 Actions 中失败的运行
2. 点击左侧的 `Build Tweak` 步骤
3. 查看错误信息

**常见解决：**
- SDK 下载失败 → 在 `.github/workflows/build.yml` 中更换 SDK 下载链接
- 缺少文件 → 检查是否所有文件都上传了

### 问题 2：找不到 .deb 文件

**检查：**
1. Actions 是否成功完成（绿色 ✅）
2. 是否展开了 Artifacts 区域
3. 有些浏览器会拦截下载，换个浏览器试试

### 问题 3：安装后提示依赖错误

**解决：**
1. 在 Sileo 搜索安装 `libroot`（包名：`com.opa334.libroot`）
2. 重新安装 Axon Reborn

### 问题 4：手势清除没反应

**解决：**
1. 设置 → 找到 Axon Reborn
2. 确保开启了：
   - `Swipe Up: Clear App` ✅
   - `Swipe Down: Clear All` ✅

---

## 📁 文件清单确认

上传完成后，你的仓库应该包含这些文件：

```
Axon-Reborn/
├── .github/
│   └── workflows/
│       └── build.yml          ← GitHub Actions 配置
├── layout/
│   └── DEBIAN/
│       └── control            ← 包信息
├── Preferences/
│   ├── Makefile
│   ├── entry.plist
│   ├── AXNRootListController.h
│   ├── AXNRootListController.m
│   └── Resources/
│       └── Root.plist
├── Tweak/
│   ├── Tweak.xm               ← 主注入文件
│   ├── Tweak.h
│   ├── AXNView.h
│   ├── AXNView.m
│   ├── AXNManager.h
│   ├── AXNManager.m
│   ├── AXNAppCell.h
│   ├── AXNAppCell.m           ← 手势核心代码
│   ├── AXNRequestWrapper.h
│   ├── AXNRequestWrapper.m
│   ├── RandomHeaders.h
│   └── Protocol.h
├── .gitignore
├── Makefile                   ← 主编译配置
└── README.md
```

---

## 🎯 快速检查清单

- [ ] 创建 GitHub 仓库
- [ ] 上传所有文件
- [ ] 创建 `.github/workflows/build.yml`
- [ ] 修改 README.md 触发编译
- [ ] 等待 Actions 完成（绿色 ✅）
- [ ] 下载 Artifacts 中的 deb
- [ ] 用 Filza 安装到 iPhone
- [ ] 在设置中开启手势开关
- [ ] 测试上滑/下滑清除通知

---

## 💡 提示

- 每次修改代码后，Actions 会自动重新编译
- 可以多次触发编译，每次都会生成新的 deb
- 如果编译一直失败，可以把 Actions 日志截图发给我，我帮你排查
