# Axon Reborn - 本地手动编译指南

## 环境要求

- macOS 10.15+ 或 Linux (Ubuntu 20.04+)
- Xcode Command Line Tools (macOS)
- 约 2GB 磁盘空间

---

## 第一步：安装 Theos

### macOS

```bash
# 1. 安装 Homebrew（如果还没有）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. 安装依赖
brew install dpkg ldid xz

# 3. 安装 Theos
bash -c "$(curl -fsSL https://raw.githubusercontent.com/theos/theos/master/bin/install-theos)"

# 4. 验证安装
echo $THEOS
# 应该输出 /opt/theos 或你自定义的路径
```

### Linux (Ubuntu/Debian)

```bash
# 1. 安装依赖
sudo apt-get update
sudo apt-get install -y build-essential git curl xz-utils ldid

# 2. 创建 Theos 目录
export THEOS=~/theos

# 3. 克隆 Theos
git clone --recursive https://github.com/theos/theos.git $THEOS

# 4. 添加到环境变量
echo 'export THEOS=~/theos' >> ~/.bashrc
echo 'export PATH=$THEOS/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

---

## 第二步：下载 iOS SDK

```bash
# 创建 SDK 目录
mkdir -p $THEOS/sdks

# 下载 iOS 14.5 SDK（推荐）
cd $THEOS/sdks
curl -LO https://github.com/chrisharper22/sdks/releases/download/iOS14.5/iPhoneOS14.5.sdk.tar.gz

# 解压
tar -xzf iPhoneOS14.5.sdk.tar.gz
rm iPhoneOS14.5.sdk.tar.gz

# 验证
ls $THEOS/sdks/
# 应该看到 iPhoneOS14.5.sdk 文件夹
```

**备用 SDK 下载源：**
- https://github.com/xybp888/iOS-SDKs/releases
- https://github.com/chrisharper22/sdks/releases

---

## 第三步：安装 libroot

### 方法 1：从源码编译（推荐）

```bash
# 1. 克隆 libroot
git clone https://github.com/opa334/libroot.git ~/libroot
cd ~/libroot

# 2. 编译
make

# 3. 复制到 Theos
mkdir -p $THEOS/lib
cp libroot.dylib $THEOS/lib/libroothide.dylib
cp -r include/* $THEOS/include/
```

### 方法 2：手动下载（简单）

```bash
# 直接下载预编译库
mkdir -p $THEOS/lib
curl -L -o $THEOS/lib/libroothide.dylib   https://raw.githubusercontent.com/opa334/libroot/main/libroothide.dylib
```

---

## 第四步：编译 Axon Reborn

```bash
# 1. 进入项目目录
cd ~/Axon-Reborn  # 或你解压的路径

# 2. 清理旧编译文件
make clean

# 3. 开始编译
make package

# 4. 等待编译完成...
# 成功后会显示：
# ==> Packaging me.nepeta.axon_2.0_iphoneos-arm64.deb...
# ==> Finished packaging Axon
```

### 编译输出

编译成功后，`.deb` 文件会生成在：
```
packages/me.nepeta.axon_2.0_iphoneos-arm64.deb
```

---

## 第五步：安装到设备

### 方法 1：通过 SSH 安装（需要 WiFi）

```bash
# 1. 确保设备和电脑在同一 WiFi
# 2. 获取设备 IP（设置-WiFi-点击i）

# 3. 复制 deb 到设备
scp packages/*.deb root@你的设备IP:/var/mobile/

# 4. SSH 进入设备安装
ssh root@你的设备IP
dpkg -i /var/mobile/me.nepeta.axon_2.0_iphoneos-arm64.deb
# 或
apt install /var/mobile/me.nepeta.axon_2.0_iphoneos-arm64.deb

# 5. 重启 SpringBoard
sbreload
```

### 方法 2：通过 Filza 安装

1. 用 AirDrop/微信/QQ 把 `.deb` 传到手机
2. 用 **Filza** 打开 `.deb`
3. 点击 **安装**
4. 点击 **注销**（Respring）

### 方法 3：通过 Sileo/Zebra

1. 把 `.deb` 放到手机的 `/var/mobile/Documents/`
2. 在 Sileo/Zebra 中添加本地源：
   - `file:///var/mobile/Documents/`
3. 刷新后找到 Axon Reborn 安装

---

## 常见问题

### 1. 编译报错：`libroot/libroot.h` not found

```bash
# 解决方法：检查 libroot 是否正确安装
ls $THEOS/include/libroot/
# 如果没有，重新执行第三步
```

### 2. 编译报错：`SDK not found`

```bash
# 检查 SDK 路径
ls $THEOS/sdks/
# 确保有 iPhoneOS14.5.sdk

# 或在 Makefile 中指定 SDK
export SDKVERSION = 14.5
export SYSROOT = $(THEOS)/sdks/iPhoneOS14.5.sdk
```

### 3. 编译报错：`arm64e` 架构不支持

```bash
# 修改 Makefile，只编译 arm64
export ARCHS = arm64
```

### 4. 安装后插件不生效

```bash
# 检查依赖是否安装
apt install com.opa334.libroot

# 检查是否有冲突插件
apt remove com.thomasfinch.priorityhub  # 等冲突包
```

### 5. 手势清除没反应

- 确保在 **设置 → Axon Reborn** 中开启了手势开关
- 确保没有安装其他手势冲突的插件

---

## 调试方法

### 查看日志

```bash
# 在设备上执行
ssh root@你的设备IP
tail -f /var/log/syslog | grep Axon

# 或使用 oslog（iOS 14+）
oslog --predicate 'process == "SpringBoard"' --level debug
```

### 安全模式排查

如果安装后进入安全模式：
1. 删除插件：`dpkg -r me.nepeta.axon`
2. 检查日志找出崩溃原因
3. 修复后重新安装

---

## 完整命令速查

```bash
# 一键编译（假设环境已配置好）
cd ~/Axon-Reborn && make clean && make package

# 一键安装（假设已配置 SSH）
scp packages/*.deb root@192.168.1.100:/tmp/ && ssh root@192.168.1.100 "dpkg -i /tmp/*.deb && sbreload"

# 卸载
ssh root@192.168.1.100 "dpkg -r me.nepeta.axon && sbreload"
```
