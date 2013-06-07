
require "wx"
--require "math"

ID_START_BUTTON      = 1
ID_PAUSE_BUTTON      = 2
ID_END_BUTTON        = 3

ID_LEFT_BUTTON       = 10
ID_RIGHT_BUTTON      = 11
ID_UP_BUTTON         = 12
ID_DOWN_BUTTON       = 13

-- 方块的形状
BlockShape = {
	[1]={0,1,0,0, 0,1,0,0, 0,1,1,0, 0,0,0,0};
	[2]={0,1,0,0, 0,1,0,0, 1,1,0,0, 0,0,0,0};
	[3]={0,1,0,0, 0,1,1,0, 0,1,0,0, 0,0,0,0};
	[4]={0,0,1,0, 0,1,1,0, 0,1,0,0, 0,0,0,0};
	[5]={0,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,0,0};
	[6]={0,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0};
	[7]={0,1,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0};
}

curBlockIdx = 3
nextBlockIdx = 3
currentBlock = {}
bIsGameOver = false


width = 252
height = 316

frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "RussionBlock",
    wx.wxDefaultPosition, wx.wxSize(width, height))
--panel = wx.wxPanel(frame, wx.wxID_ANY)

timer = wx.wxTimer(frame)

local startButton = wx.wxButton( frame, ID_START_BUTTON, "&Start")
local endButton   = wx.wxButton( frame, ID_END_BUTTON, 	 "&End")
local pauseButton = wx.wxButton( frame, ID_PAUSE_BUTTON, "&Pause")

local score 		= wx.wxStaticText( frame, wx.wxID_ANY, "000",	wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxST_NO_AUTORESIZE + wx.wxALIGN_CENTER)

local leftButton 	= wx.wxButton( frame, ID_LEFT_BUTTON, 	"←")
local rightButton   = wx.wxButton( frame, ID_RIGHT_BUTTON,	"→")
local upButton 		= wx.wxButton( frame, ID_UP_BUTTON, 	"↑")
local downButton 	= wx.wxButton( frame, ID_DOWN_BUTTON, 	"↓")

local curPosX = 5
local curPosY = 1
local blockRow = 20
local blockCol = 12
local blockGrid = {}
local nScore	= 0;

local isSkipEvent = false

for r = 1, blockRow do
	for c = 1, blockCol do
		blockGrid[r * blockCol + c] = 0
	end
end

--blockGrid[1*blockCol + 5] = 1
--blockGrid[2*blockCol + 5] = 1
--blockGrid[10*blockCol + 5] = 2

function CopyGrid(src, dst)
	for r = 1, blockRow do
		for c = 1, blockCol do
			dst[r * blockCol + c] = src[r * blockCol + c]
		end
	end
end

function CopyBlock(src, dst)
	--tmpBlock = BlockShape[curBlockIdx]
	for r = 0, 3 do
		for c = 1, 4 do
			dst[r * 4 + c] = src[r * 4 + c]
		end
	end
end

function OnSize(event)

	local win = event:GetEventObject():DynamicCast("wxWindow")
	width, height = win:GetClientSizeWH()
	width = width * 2 / 3

	local cellSize = width / blockCol;
	if cellSize * (blockRow + 2) > height then
		cellSize = height / (blockRow + 2)
	end

	local x = (blockCol + 1.5) * cellSize

	local y = (height - cellSize * blockRow) / 2 + cellSize * 5

	startButton:SetSize(x, y, 60, 25); y = y+28
	pauseButton:SetSize(x, y, 60, 25); y = y+28
	endButton:SetSize(x, y, 60, 25); y = y+35

	score:SetSize(x, y, 60, 25)

	y = y+33
	upButton:SetSize(x + 19, y, 22, 22)		; y = y+27
	leftButton:SetSize(x, y, 22, 22)
	rightButton:SetSize(x + 38, y, 22, 22)	; y = y+27
	downButton:SetSize(x+ 19, y, 22, 22)

	frame:Refresh(false)
end

function Layout()
end

function OnKeyDown(event)
    event:Skip()

	local key = event:GetKeyCode()
	--print(key)

	BlockControl(key)
end

function BlockControl(key)
	if isSkipEvent then
		return
	end

	isSkipEvent = true

	local workGrid = {}
	CopyGrid(blockGrid, workGrid)

	local bRet

	if key == wx.WXK_LEFT then-- Left Key
		bRet = MoveLeft(workGrid)
	elseif key == wx.WXK_RIGHT then-- Right Key
		bRet = MoveRight(workGrid)
	elseif key == wx.WXK_DOWN then-- Down Key
		bRet = MoveDown(workGrid)
	elseif key == wx.WXK_UP then
		bRet = MoveUp(workGrid)
	end

	if bRet then
		CopyGrid(workGrid, blockGrid)
		frame:Refresh(false)
	end

	isSkipEvent = false
end



function OnPaintCellWindow(event)
    local win = event:GetEventObject():DynamicCast("wxWindow")

    -- ALWAYS create a wxPaintDC in a wxEVT_PAINT handler, even if unused
    local dc = wx.wxPaintDC(win)

	local cell = win:GetId()
	local width, height = win:GetClientSizeWH()

	--local brush = wx.wxBrush(wx.wxRED, wx.wxSOLID)

	--dc:DrawRectangle(0, 0, width / 2, height / 2)

	DrawGrid(20, 12, win, dc)

    --brush:delete()
    dc:delete() -- ALWAYS delete() any wxDCs created when done
end

function RotateBlock(currentBlock)
	currentBlock[3], currentBlock[11], currentBlock[9], currentBlock[1] = currentBlock[1], currentBlock[3], currentBlock[11], currentBlock[9]
	currentBlock[7], currentBlock[10], currentBlock[5], currentBlock[2] = currentBlock[2], currentBlock[7], currentBlock[10], currentBlock[5]
	currentBlock[4], currentBlock[8], currentBlock[12], currentBlock[13], currentBlock[14], currentBlock[15] = currentBlock[13], currentBlock[14], currentBlock[15], currentBlock[4], currentBlock[8], currentBlock[12]
end
function DrawGrid(row, col, win, dc)
	local width, height = win:GetClientSizeWH()
	width = width * 2 / 3

	local cellSize = width / col;
	if cellSize * (row + 2) > height then
		cellSize = height / (row + 2)
	end

	cellSize = math.floor(cellSize)
	local yMargin = (height - cellSize * row) / 2 - cellSize

	dc:DrawRectangle(cellSize -3, cellSize + yMargin -3, col * cellSize+7, row * cellSize+7)
	dc:DrawRectangle((col + 1.8) * cellSize -3, cellSize + yMargin -3, 4 * cellSize+7, 4 * cellSize+7)

	-- 画网格线
	local pen = wx.wxPen(wx.wxColour(220, 220, 220), 1, wx.wxSOLID)
	dc:SetPen(pen)

	for r = 1, row do
		local y = r * cellSize + yMargin
		for c = 1, col do
			local x = c * cellSize
			dc:DrawRectangle(x, y, cellSize+1, cellSize+1)
		end
	end


	for r = 1, 4 do
		local y = r * cellSize + yMargin
		for c = 1, 4 do
			local x = (col + c + 0.8) * cellSize
			dc:DrawRectangle(x, y, cellSize+1, cellSize+1)
		end
	end

	-- 画方块
	local brushMove = wx.wxBrush(wx.wxRED, wx.wxSOLID)
	local brushStay = wx.wxBrush(wx.wxBLUE, wx.wxSOLID)

	dc:SetBrush(brushStay)

	for r = 1, row do
		local y = r * cellSize + yMargin
		for c = 1, col do
			if blockGrid[r * col + c] == 2 then
				local x = c * cellSize
				dc:DrawRectangle(x+1, y+1, cellSize-1, cellSize-1)
			end
		end
	end

	pen:SetColour(wx.wxColour(000, 250, 50))
	dc:SetBrush(brushMove)
	dc:SetPen(pen)
	for r = 1, row do
		local y = r * cellSize + yMargin
		for c = 1, col do
			if blockGrid[r * col + c] == 1 then
				local x = c * cellSize
				dc:DrawRectangle(x+1, y+1, cellSize-2, cellSize-2)
			end
		end
	end

	-- 画方块预览
	tmpBlock = BlockShape[nextBlockIdx]
	for r = 1, 4 do
		for c = 1, 4 do
			local y = r * cellSize + yMargin
			local x = (col + c + 0.8) * cellSize
			local i = (r - 1) * 4 + c
			if tmpBlock[i] == 1 then
				dc:DrawRectangle(x+1, y+1, cellSize-2, cellSize-2)
			end
		end
	end

	pen:delete();

    brushMove:delete()
	brushStay:delete()
end

function OnTimer(event)
	if isSkipEvent then
		return
	end

	isSkipEvent = true

	local workGrid = {}
	CopyGrid(blockGrid, workGrid)

	local bRet = MoveDown(workGrid)

	if bRet then
		bRet = ValidateBlock(workGrid)
	end

	if bRet then
		CopyGrid(workGrid, blockGrid)
	else
		FixupBlock(blockGrid)
		CountScore()
		bRet = PrepareNextBlock()
		if bRet == false then
			bIsGameOver = true
			timer:Stop();
			startButton:Enable(true)
			endButton:Enable(false)
			pauseButton:Enable(false)
			frame:Refresh(false)
			print("Game Over")
		end
	end

	frame:Refresh(false)

	isSkipEvent = false
end

function PrepareNextBlock()
	curPosX = 5
	curPosY = 1

	curBlockIdx = nextBlockIdx
	CopyBlock(BlockShape[curBlockIdx], currentBlock)

	nextBlockIdx = math.random(7)

	return CombineBlock(blockGrid, currentBlock)
end

function MoveDown(workGrid)
	-- 检查是否到最低边了
	for c = 1, blockCol do
		if workGrid[blockRow * blockCol + c] == 1 then
			return false
		end
	end

	for c = 1, blockCol do
		for r = (blockRow - 1) * blockCol, blockCol, -blockCol do
			if workGrid[r + c] == 1 then
				workGrid[r + blockCol + c] = workGrid[r + blockCol + c] + 1
				workGrid[r + c] = 0
			end
		end
	end

	bRet = ValidateBlock(workGrid)

	if bRet then
		curPosY = curPosY + 1
	end

	return bRet
end

function MoveLeft(workGrid)
	-- 检查是否到最左边了
	for r = 1, blockRow do
		if workGrid[r * blockCol + 1] == 1 then
			return false
		end
	end

	for r = blockCol, blockRow * blockCol, blockCol do
		for c = 2, blockCol do
			if workGrid[r + c] == 1 then
				workGrid[r + c - 1] = workGrid[r + c - 1] + 1
				workGrid[r + c] = 0
			end
		end
	end

	bRet = ValidateBlock(workGrid)

	if bRet then
		curPosX = curPosX - 1
	end

	return bRet
end

function MoveRight(workGrid)
	-- 检查是否到最右边了
	for r = 1, blockRow do
		if workGrid[r * blockCol + blockCol] == 1 then
			return false
		end
	end

	for r = blockCol, blockRow * blockCol, blockCol do
		for c = blockCol - 1, 1, -1 do
			if workGrid[r + c] == 1 then
				workGrid[r + c + 1] = workGrid[r + c + 1] + 1
				workGrid[r + c] = 0
			end
		end
	end

	bRet = ValidateBlock(workGrid)

	if bRet then
		curPosX = curPosX + 1
	end

	return bRet
end

function MoveUp(workGrid)
	if curBlockIdx == 6 then
		--正方形的不需要旋转
		return false
	end

	local workBlock = {}
	CopyBlock(currentBlock, workBlock)
	RotateBlock(workBlock)
	if curBlockIdx >= 4 and curBlockIdx <=5 then
		-- 再旋转两次
		if workBlock[1] + workBlock[5] + workBlock[9] > 1 then
			RotateBlock(workBlock)
			RotateBlock(workBlock)
		end
	end

	bRet = CombineBlock(workGrid, workBlock)

	if bRet then
		CopyBlock(workBlock, currentBlock)
		bRet = ValidateBlock(workGrid)
	end

	return bRet
end

function CombineBlock(workGrid, workBlock)
	-- 清除原来活动的方块
	for r = blockCol, blockRow * blockCol, blockCol do
		for c = 1, blockCol do
			if workGrid[r + c] == 1 then
				workGrid[r + c] = 0
			end
		end
	end

	bResult = true
	for r = 0, 3 do
		for c = 1, 4 do
			rr = curPosY + r
			cc = curPosX + c - 1
			if workBlock[r * 4 + c] > 0 then
				if rr > blockRow or cc > blockCol or cc < 1 then
					return false
				end
				workGrid[rr * blockCol + cc] = workGrid[rr * blockCol + cc] + workBlock[r * 4 + c]

				--检查是否无法再进行了
				if workGrid[rr * blockCol + cc] > 1 then
					workGrid[rr * blockCol + cc] = 1
					bResult =  false
				end
			end
		end
	end

	return bResult
end

function ValidateBlock(workGrid)
	for r = 1, blockRow do
		for c = 1, blockCol do
			if workGrid[r * blockCol + c] > 2 then
				return false
			end
		end
	end

	return true
end

function FixupBlock(workGrid)
	for r = 1, blockRow do
		for c = 1, blockCol do
			if workGrid[r * blockCol + c] > 0 then
				workGrid[r * blockCol + c] = 2
			end
		end
	end
end

function CountScore()
	local curScore = 0
	local row = blockRow
	while row > 1 do
		local bCanContinue = true
		for c = 1, blockCol do
			if blockGrid[row * blockCol + c] ~=2 then
				bCanContinue = false
				break
			end
		end

		if bCanContinue then
			curScore = curScore + 1

			--整个往下移一行
			for r = row, 2, -1 do
				for c = 1, blockCol do
					blockGrid[r * blockCol  + c] = blockGrid[(r - 1) * blockCol + c]
				end
			end
		else
			row = row -1
		end
	end

	if curScore == 1 then
		nScore = nScore + 1
	elseif curScore == 2 then
		nScore = nScore + 3
	elseif curScore == 3 then
		nScore = nScore + 6
	elseif curScore == 4 then
		nScore = nScore + 10
	end

	score:SetLabel(string.format("%03d", nScore * 100))
end


	frame:Connect(wx.wxEVT_KEY_DOWN,    OnKeyDown )
	--frame:Connect(wx.wxEVT_ERASE_BACKGROUND, function(event) end)
	frame:Connect(wx.wxEVT_PAINT,       OnPaintCellWindow)
	frame:Connect(wx.wxEVT_SIZE,		OnSize)
	frame:Connect(wx.wxEVT_TIMER, 		OnTimer)

frame:Connect(ID_START_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event)
		startButton:Enable(false)
		endButton:Enable(true)
		pauseButton:Enable(true)
		frame:SetFocus()

		if bIsGameOver then
			for r = 1, blockRow do
				for c = 1, blockCol do
					blockGrid[r * blockCol + c] = 0
				end
			end

			PrepareNextBlock()
		end

		nScore = 0
		score:SetLabel(string.format("%03d", nScore * 100))

		timer:Start(300);
		bIsGameOver = false
	end)

frame:Connect(ID_PAUSE_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event)
		timer:Stop();
		startButton:Enable(true)
		endButton:Enable(false)
		frame:SetFocus()
	end)

frame:Connect(ID_END_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event)
		timer:Stop();
		bIsGameOver = true
		startButton:Enable(true)
		endButton:Enable(false)
		pauseButton:Enable(false)
		frame:SetFocus()
	end)

frame:Connect(ID_LEFT_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event) BlockControl(wx.WXK_LEFT); frame:SetFocus()  end)
frame:Connect(ID_RIGHT_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event) BlockControl(wx.WXK_RIGHT); frame:SetFocus()  end)
frame:Connect(ID_DOWN_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event) BlockControl(wx.WXK_DOWN); frame:SetFocus()  end)
frame:Connect(ID_UP_BUTTON, 	wx.wxEVT_COMMAND_BUTTON_CLICKED,
    function(event) BlockControl(wx.WXK_UP); frame:SetFocus()  end)

frame:SetBackgroundColour(wx.wxColour(250, 220, 240))
frame:Show(true)
frame:SetFocus()

math.randomseed(math.random(10000))
nextBlockIdx = math.random(7)
PrepareNextBlock()

wx.wxGetApp():MainLoop()
